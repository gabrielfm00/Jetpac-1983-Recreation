#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

void PlayerFrameControl(EntityInfo* plyr, esat::SpriteHandle* comb, unsigned char fps){
  for(int i = 0; i < kNPlayerAmount; i++){
    if((plyr + i)->alive){
      if(esat::IsSpecialKeyDown(esat::kSpecialKey_Right) 
        && (plyr + i)->pos.scale_x != -1.0f){
        (plyr + i)->pos.scale_x = (plyr + i)->pos.scale_x * -1.0f;
        (plyr + i)->pos.sprite_origin_x = (plyr + i)->width;
      } else {
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Left) 
          && (plyr + i)->pos.scale_x != 1.0f){
          (plyr + i)->pos.scale_x = (plyr + i)->pos.scale_x * -1.0f;
          (plyr + i)->pos.sprite_origin_x = 0.0f;
        }
      }
    }
  }
}

void MovePlayer(EntityInfo* plyr, EntityInfo* flrs, unsigned char fps){
  for(int i = 0; i < kNPlayerAmount; i++){
    if(going_down){
      (plyr + i)->pos.y += g_gravity;
    }
    if((plyr + i)->alive){
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)
      && go_right){
        (plyr + i)->pos.x += (plyr + i)->velocity.x;
          if(g_frames%(fps / 10) == 0){
            ++(plyr + i)->sprite %= 4;
          }
      } else{
        if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)
        && go_left){
          (plyr + i)->pos.x -= (plyr + i)->velocity.x;
          if(g_frames%(fps / 10) == 0){
            ++(plyr + i)->sprite %= 4;
          }
        }
      }
      if((plyr + i)->pos.x > kNWindowWidth){
        (plyr + i)->pos.x -= kNWindowWidth;
      } else {
        if ((plyr + i)->pos.x < 0)
        {
          (plyr + i)->pos.x += kNWindowWidth;
        }
      }
        if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)
          && !bounce_off){
            (plyr + i)->pos.y -= (plyr + i)->velocity.y;
            g_head_sprite = 1;
            (plyr + i)->sprite = 5;
            if(g_frames%(fps / 7) == 0){
              ++(plyr + i)->sprite %= 8;
            }
        }
    }
  }
  if(plyr->id >= 1500){
    plyr->id = 0;
    plyr->lives += 1;
  }
}

void PlayerShooting(EntityInfo* bullt, EntityInfo* plyr, 
                    MenuColors color, double dt){

  for(int i = 0; i < kNBulletAmount; i++){
    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Space) && plyr->alive){
          plyr->shooting = true;
          g_color_of_bullet = rand() % 5;
          (bullt + i)->pos.y = (plyr + 0)->pos.y + 6.0f;
          if(plyr->pos.scale_x != 1){
            (bullt + i)->pos.x = (plyr + 0)->pos.x + 32.0f;
          }
          if(plyr->pos.scale_x != (-1)){
            (bullt + i)->pos.x = (plyr + 0)->pos.x - 32.0f;
          }
    }
  }

  if(plyr->shooting){
    g_elapsed_shot_time += dt * 0.1;
    if(g_elapsed_shot_time >= (bullt + g_current_shot)->cooldown){
      g_elapsed_shot_time -= (bullt + g_current_shot)->cooldown;
      g_current_shot++;
      if(g_current_shot >= kNBulletAmount){
        plyr->shooting = false;
        g_current_shot = 0;
      }
    }
    for(int i = 0; i < g_current_shot; i++){
      if(plyr->pos.scale_x != 1){
        (bullt + i)->pos.x += (bullt + i)->velocity.x * dt * 0.1;
      }
      if(plyr->pos.scale_x != (-1)){
        (bullt + i)->pos.x -= (bullt + i)->velocity.x * dt * 0.1;
      }
    }
  }

  for(int i = 0; i < kNBulletAmount; i++){
    esat::Vec2 *points;
    points = InitiatePoints();
    ShapeMaker((bullt + i), points);
    esat::DrawSetStrokeColor(color.r,
                            color.g,
                            color.b, 255);
    esat::DrawSetFillColor(color.r,
                          color.g,
                          color.b, 255);
    if(plyr->shooting){
      esat::DrawSolidPath((float*)points, 5);
    }

    if((bullt + i)->pos.x >= kNWindowWidth){
      (bullt + i)->pos.x -= kNWindowWidth;
    } 
    if((bullt + i)->pos.x <= 0){
      (bullt + i)->pos.x += kNWindowWidth;
    }
  }
}

void SelectBulletColor(MenuColors* color){
  switch(g_color_of_bullet){
    case 0:
      color->r = 255;
      color->g = 0;
      color->b = 0;
    break;
    case 1:
      color->r = 0;
      color->g = 255;
      color->b = 0;
    break;
    case 2:
      color->r = 0;
      color->g = 0;
      color->b = 255;
    break;
    case 3:
      color->r = 255;
      color->g = 255;
      color->b = 255;
    break;
    case 4:
      color->r = 127;
      color->g = 0;
      color->b = 255;
    break;
  }
}

void TwoPlayerGameLoop(EntityInfo* plyr, EntityInfo* plyr_2, 
                       EntityInfo* enem, double dt){
  if(!plyr->alive && g_state_of_game == 0
     && !is_level_changing){
    g_player_timer += dt * 0.001;
  }
  if(!plyr_2->alive && g_state_of_game == 1
     && !is_level_changing){
    g_player_timer_2 += dt * 0.001;
  }
  if(!plyr->alive && g_state_of_game == 0 
    && g_player_timer >= 2.5 && !is_level_changing){
    g_state_of_game = 1;
    g_player_timer -= g_player_timer;
    for(int i = 0; i < kNEnemAmount; i++){
      (enem + i)->alive = false;
    }
  }
  if(!is_level_changing){
    if(!plyr_2->alive && g_state_of_game == 1 
      && g_player_timer_2 >= 2.5){
      g_state_of_game = 0;
      g_player_timer_2 -= g_player_timer_2;
      for(int i = 0; i < kNEnemAmount; i++){
        (enem + i)->alive = false;
      }
    }
  }
}

void ResetLevels(LevelHandling* lvl, LevelHandling* lvl_2, 
                EntityInfo* a_craft, EntityInfo* a_craft_2){
  if(lvl->level >= 7){
    lvl->level = 0;
  }
  if(lvl->ship_level >= 12){
    lvl->ship_level = 0;
    a_craft->id = 0;
  }
  if(lvl_2->level >= 7){
    lvl_2->level = 0;
  }
  if(lvl_2->ship_level >= 12){
    lvl_2->ship_level = 0;
    a_craft_2->id = 0;
  }
}