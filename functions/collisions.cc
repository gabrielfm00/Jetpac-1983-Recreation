#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

bool SegmentCollision(
    float const a_start,
    float const a_end,
    float const b_start,
    float const b_end){
      
  return 
      a_start <= b_start && (b_start <= a_end || b_end <= a_end)
      || b_start < a_start && (a_start <= b_end || a_end <= b_end);
}

bool CheckCollision(EntityInfo a, EntityInfo b){
  //Bounding box:
  return SegmentCollision(
             a.pos.x, a.pos.x + a.width,
             b.pos.x, b.pos.x + b.width)
         && SegmentCollision(
                a.pos.y, a.pos.y + a.height,
                b.pos.y, b.pos.y + b.height);

}

bool PointCollision(EntityInfo a, esat::Vec2 b){

    
    return (b.x >= a.pos.x && b.x <= a.pos.x + a.width 
            && b.y >= a.pos.y && b.y <= a.pos.y + a.height); 
}

bool TopBottomCollision(EntityInfo a, EntityInfo b){

    return ((a.pos.x + a.width >= b.pos.x)) && ((a.pos.x <= b.pos.x + b.width)) && 
            ((a.pos.y + a.height >= b.pos.y)) && ((a.pos.y <= b.pos.y + b.height));
}

void ExplosionDeathAnimation(EntityInfo* enem, esat::SpriteHandle* exp,
                             unsigned char fps, double dt){
    for(int i = 0; i < kNEnemAmount; i++){
        if(!(enem + i)->alive && !is_level_changing){
            (enem + i)->timer += dt * 0.001;
            if((enem + i)->timer <= 0.5){
                esat::DrawSprite(*(enem->color.colors + 
                                    (enem + i)->lives), 
                                    (enem + i)->pos);
                if((enem + i)->spawn_point == 1){
                    esat::DrawSprite(*(enem->color.colors + (enem + i)->lives), 
                                    (enem + i)->pos.x - 28, 
                                    (enem + i)->pos.y);
                } else {
                    if((enem + i)->spawn_point == 0){
                        esat::DrawSprite(*(enem->color.colors + (enem + i)->lives),
                                            (enem + i)->pos.x + 28,
                                            (enem + i)->pos.y);
                    }
                }
                esat::DrawSprite(*(exp + g_explosion_frames), (enem + i)->pos);
                if(g_frames%(fps / 5) == 0){
                    ++g_explosion_frames %= 3;
                }
            } else {
                (enem + i)->timer -= (enem + i)->timer;
            }
        }
    }
    
}

void EnemyGroundCollision(EntityInfo* enem, EntityInfo* flrs,
                          EntityInfo* sd_terrain, LevelHandling* lvl){
    for(int i = 0; i < kNEnemAmount; i++){
        for(int j = 0; j < kNFloorAmount; j++){
            if(CheckCollision(*(enem + i), *(flrs + j)) 
              && (enem + i)->alive && 
              (lvl->level == 0 || lvl->level == 3 || lvl->level == 6)){
                (enem + i)->alive = false;
            }
            if(TopBottomCollision(*(enem + i), *(flrs + j))
                && (lvl->level == 1 || lvl->level == 2 || lvl->level == 4
                    || lvl->level == 5 || lvl->level == 7)){
                        (enem + i)->angle = -(enem + i)->angle;
            }
        }
        for(int s = 0; s < 6; s++){
            if(CheckCollision(*(sd_terrain + s), *(enem + i))
                && (lvl->level == 1 || lvl->level == 2 || lvl->level == 4
                    || lvl->level == 5 || lvl->level == 7)){
                    (enem + i)->angle = M_PI - (enem + i)->angle;
            }
            if(CheckCollision(*(enem + i), *(sd_terrain + s)) 
              && (enem + i)->alive && 
              (lvl->level == 0 || lvl->level == 3 || lvl->level == 6)){
                (enem + i)->alive = false;
            }
        }
        if((enem + i)->pos.x > kNWindowWidth + 20 
            && (enem + i)->spawn_point == 0){
            (enem + i)->pos.x -= kNWindowWidth;
        } else {
            if((enem + i)->pos.x < -20 
                && (enem + i)->spawn_point == 1){
                (enem + i)->pos.x += kNWindowWidth;
            }
        }
    }
}

void CollideShipPart(EntityInfo* plyr, EntityInfo* a_craft,
                     EntityInfo* bec, EntityInfo* flcl){
    if(CheckCollision(*(plyr + 0), *(a_craft + 1))
       && !CheckCollision(*(a_craft + 1), *bec)
       && plyr->alive){
       (a_craft + 1)->pos.x = (plyr + 0)->pos.x;
       (a_craft + 1)->pos.y = (plyr + 0)->pos.y;
    }
    if(CheckCollision(*(plyr + 0), *(a_craft + 2)) 
        && TopBottomCollision(*(a_craft + 1), *(a_craft + 0))
        && !CheckCollision(*(a_craft + 2), *bec)
        && plyr->alive){
        (a_craft + 2)->pos.x = (plyr + 0)->pos.x;
        (a_craft + 2)->pos.y = (plyr + 0)->pos.y;
    }
    if(CheckCollision(*bec, *(a_craft + 1))){
        (a_craft + 1)->pos.x = bec->pos.x;
    }
    if(!TopBottomCollision(*(a_craft + 1), *(a_craft + 0)) 
        && CheckCollision(*bec, *(a_craft + 1))){
        (a_craft + 1)->pos.y += g_ship_gravity;
    }
    if(CheckCollision(*bec, *(a_craft + 2))){
        (a_craft + 2)->pos.x = bec->pos.x;
    }
    if(!TopBottomCollision(*(a_craft + 2), *(a_craft + 1)) 
        && CheckCollision(*bec, *(a_craft + 2))){
        (a_craft + 2)->pos.y += g_ship_gravity; 
        a_craft->assembled = true;
    }
    if(CheckCollision(*(a_craft + 1), *(a_craft + 0))
    && (a_craft + 1)->alive){
        (a_craft + 1)->alive = false;
        plyr->score += a_craft->score;
        plyr->id += a_craft->score;
    }
    if(CheckCollision(*(a_craft + 2), *(a_craft + 1))
    && (a_craft + 2)->alive){
        (a_craft + 2)->alive = false;
        plyr->score += a_craft->score;
        plyr->id += a_craft->score;
    }
    if(CheckCollision(*(a_craft + 2), *(a_craft + 1))){
        flcl->shooting = true;
    }
}

void PicakblesCollide(EntityInfo* plyr, EntityInfo* ex_points,
                      EntityInfo* flrs){
    if((CheckCollision(*(plyr + 0), *ex_points) 
        || CheckCollision(*(plyr + 1), *ex_points))
        && ex_points->alive){
            ex_points->alive = false;
            ex_points->pos.x = (float)(32 + rand() % (kNWindowWidth - 32));
            ex_points->pos.y = 60.0f;
            ex_points->sprite = rand() % 4;
            ex_points->lives = rand() % 7;
            ex_points->spawn_point = 3 + rand() % 10;
            ex_points->velocity.y = 5.0f;
            plyr->score += ex_points->score;
            plyr->id += ex_points->score;
    }
    for(int i = 0; i < kNFloorAmount; i++){
        if(CheckCollision(*ex_points, *(flrs + i))){
            ex_points->velocity.y = 0.0f;
        }
    }
}

void FuelCollide(EntityInfo* flrs, EntityInfo* flcl, 
                     EntityInfo* a_craft, EntityInfo* plyr,
                     EntityInfo* bec){
    for(int i = 0; i < kNFloorAmount; i++){
        if(CheckCollision(*flcl, *(flrs + i))){
            flcl->velocity.y = 0.0f;
        }
    }
    if(CheckCollision(*flcl, *(plyr + 0)) && flcl->alive
     && !CheckCollision(*flcl, *bec)){
        flcl->pos.x = (plyr + 0)->pos.x;
        flcl->pos.y = (plyr + 0)->pos.y;
    }
    if(CheckCollision(*flcl, *bec) && flcl->alive){
        flcl->pos.x = bec->pos.x;
        flcl->velocity.y = 5.0f;
    }
    if(CheckCollision(*flcl, *(a_craft + 0)) && flcl->alive){
        flcl->alive = false;
        flcl->fuel_count += 1;
        flcl->pos.x = (float)(32 + rand() % (kNWindowWidth - 32));
        flcl->pos.y = 60.0f;
        plyr->score += flcl->score;
        plyr->id += flcl->score;
    }
}

void LevelChange(EntityInfo* a_craft, EntityInfo* plyr, 
                EntityInfo* enem, EntityInfo* flrs, 
                EntityInfo* flcl, LevelHandling* lvl, double dt){
    if(CheckCollision(*(plyr + 0), *(a_craft + 0)) 
       && flcl->fuel_count >= 6){
        a_craft->start_going_up = true;
        is_level_changing = true;
    }
    for(int i = 0; i < kNShipAmount; i++){
        if(a_craft->start_going_up 
           && (a_craft + 2)->pos.y >= (-80.0f - 32.0f)){
            (a_craft + i)->pos.y -= 6.0f;
            plyr->alive = false;
        } 
        if((a_craft + 2)->pos.y <= (-80.0f - 32.0f)){
            a_craft->start_going_up = false;
            for(int j = 0; j < kNEnemAmount; j++){
                (enem + j)->alive = false;
            }
            flcl->fuel_count = 0;
        }
        if(!a_craft->start_going_up 
            && !CheckCollision(*(a_craft + 0), *(flrs + 3))){
                g_elapsed_level_change += dt * 0.001;
                (a_craft + i)->pos.y += 4.0f;
        }
    }
        if(!a_craft->start_going_up && 
           g_elapsed_level_change >= 16.5f){
            g_elapsed_level_change -= 16.5f;
            lvl->ship_level += 1;
            lvl->level += 1;
            is_level_changing = false;
            SetPlayerInfo(plyr);
        }
}

void PlayerCollideEnemy(EntityInfo* plyr, EntityInfo* enem, 
                        double dt, int state_game){
    for(int j = 0; j < kNPlayerAmount; j++){
        for(int i = 0; i < kNEnemAmount; i++){
            if((CheckCollision(*(plyr + 0), *(enem + i))
            || CheckCollision(*(plyr + 1), *(enem + i)))
            && (enem + i)->alive && (plyr + 0)->alive && (plyr + 1)->alive){
                (enem + i)->alive = false;
                (plyr + j)->alive = false;
                plyr->lives -= 1;
            }
        }
            if(!(plyr + j)->alive){
                (plyr + j)->rev_timer += dt * 0.001;
            }
            if((plyr + j)->rev_timer >= 3.5
            && !is_level_changing){
                (plyr + j)->rev_timer -= (plyr + j)->rev_timer;
                SetPlayerInfo(plyr);
            }
    }
}

void ShipCollideWithFloor(EntityInfo* a_craft, EntityInfo* flrs, EntityInfo* bec){

  if((!CheckCollision(*(flrs + 0), *(a_craft + 1))
    && !CheckCollision(*(flrs + 1), *(a_craft + 1))
    && !CheckCollision(*(flrs + 2), *(a_craft + 1))
    && !CheckCollision(*(flrs + 3), *(a_craft + 1)))
    && !CheckCollision(*bec, *(a_craft + 1))
    && !a_craft->assembled && !a_craft->start_going_up){
    (a_craft + 1)->pos.y += (a_craft + 1)->velocity.y;
  }
    if((!CheckCollision(*(flrs + 0), *(a_craft + 2))
    && !CheckCollision(*(flrs + 1), *(a_craft + 2))
    && !CheckCollision(*(flrs + 2), *(a_craft + 2))
    && !CheckCollision(*(flrs + 3), *(a_craft + 2)))
    && !CheckCollision(*bec, *(a_craft + 2))
    && !a_craft->assembled && !a_craft->start_going_up){
    (a_craft + 2)->pos.y += (a_craft + 1)->velocity.y;
  }
  
}

void PlayerFloorCollide(EntityInfo* flrs, EntityInfo* plyr, EntityInfo* sd_terrain){
        if((!CheckCollision(*(flrs + 0), *(plyr + 0))
        && !CheckCollision(*(flrs + 1), *(plyr + 0))
        && !CheckCollision(*(flrs + 2), *(plyr + 0))
        && !CheckCollision(*(flrs + 3), *(plyr + 0)))){
            going_down = true; 
        } else { 
            going_down = false; 
        }
        
        if((!CheckCollision(*(flrs + 0), *(plyr + 1))
        && !CheckCollision(*(flrs + 1), *(plyr + 1))
        && !CheckCollision(*(flrs + 2), *(plyr + 1))
        && !CheckCollision(*(flrs + 4), *(plyr + 1)))){
            bounce_off = false;
        } else {
            bounce_off = true;
        }
        for(int i = 0; i < kNPlayerAmount; i++){
            if((CheckCollision(*(sd_terrain + 1), *(plyr + i))
                || CheckCollision(*(sd_terrain + 3), *(plyr + i))
                || CheckCollision(*(sd_terrain + 5), *(plyr + i)))){
                    go_left = false;
            } else {
                go_left = true;
            }
            if((CheckCollision(*(sd_terrain + 0), *(plyr + i))
                || CheckCollision(*(sd_terrain + 2), *(plyr + i))
                || CheckCollision(*(sd_terrain + 4), *(plyr + i)))){
                    go_right = false;
            } else {
                go_right = true;
            }
        }
}

void LaserCollideEnemy(EntityInfo* plyr, EntityInfo* enem, EntityInfo* bullt){
    for(int i = 0; i < kNEnemAmount; i++){
        for(int j = 0; j < kNBulletAmount; j++){
            if(CheckCollision(*(bullt + j), *(enem + i))
            && (enem + i)->alive && plyr->shooting
            && ((enem + i)->pos.x > 0 || (enem + i)->pos.x < kNWindowWidth)){
                (enem + i)->alive = false;
                plyr->score += enem->score;
                plyr->id += enem->score;
            }
        }
    }

}