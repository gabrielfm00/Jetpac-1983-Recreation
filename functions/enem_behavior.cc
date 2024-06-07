#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

esat::Vec2 Vec2Sum(esat::Vec2 a, esat::Vec2 b){

  return { {a.x + b.x}, {a.y + b.y} };
}

esat::Vec2 Vec2Sub(esat::Vec2 a, esat::Vec2 b){

  return { {a.x - b.x}, {a.y - b.y} };
}

esat::Vec2 Vec2MultScale(esat::Vec2 p1, float scale){

  return { {p1.x * scale}, {p1.y * scale} };
}

float Vec2Length(esat::Vec2 g){

  return sqrtf(g.x * g.x + g.y * g.y);
}

esat::Vec2 Vec2Normalization(esat::Vec2 g){
    float length = Vec2Length(g);
    esat::Vec2 tmp = {
        g.x / length,
        g.y / length
    };
    return tmp;
}


void SideToSideMov(EntityInfo* enem, LevelHandling* lvl){
    if(lvl->level == 0 || lvl->level == 3){
        switch(enem->dir){
            case 0:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
            break;
            case 1:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
                enem->pos.y += enem->velocity.y * sinf(enem->angle);
            break;
            case 2:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
            break;
            case 3:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
                enem->pos.y += enem->velocity.y * sinf(enem->angle);
            break;
        }
    }
}

void OneMov(EntityInfo* enem, LevelHandling* lvl){
    if(lvl->level == 4 || lvl->level == 7
        && !enem->inside){
        switch(enem->dir){
            case 0:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
            break;
            case 1:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
            break;
        }
    }
}

void ImLazy(EntityInfo* enem, LevelHandling* lvl){
    if(lvl->level == 6){
        switch(enem->dir){
            case 0:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
            break;
            case 1:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
            break;
        }
    }
}

void DifferentStart(EntityInfo* enem, LevelHandling* lvl){
    if(lvl->level == 1 || lvl->level == 2 || lvl->level == 5){
        switch(enem->dir){
            case 0:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
                enem->pos.y += enem->velocity.y * sinf(enem->angle);
            break;
            case 1:
                enem->pos.x += enem->velocity.x * cosf(enem->angle);
                enem->pos.y -= enem->velocity.y * sinf(enem->angle);
            break;
            case 2:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
                enem->pos.y -= enem->velocity.y * sinf(enem->angle);
            break;
            case 3:
                enem->pos.x -= enem->velocity.x * cosf(enem->angle);
                enem->pos.y += enem->velocity.y * sinf(enem->angle);
            break;
        }
    }
}

void SpawnPoint(EntityInfo* enem, LevelHandling* lvl){
    switch(enem->spawn_point){
        case 0:
            enem->pos.x = -64;
            enem->pos.y = 100 + (rand() % 400);
            enem->pos.scale_x = 1.0f;
            enem->dir = rand() % 2;
            if((lvl->level == 4 || lvl->level == 7)
                && enem->spawn_point == 0){
                enem->dir = 0;
            }
            if(lvl->level == 6 && enem->spawn_point == 0){
                enem->dir = 0;
            }
        break;
        case 1:
            enem->pos.x = kNWindowWidth + 64;
            enem->pos.y = 100 + (rand() % 400);
            enem->pos.scale_x = -1.0f;
            enem->pos.sprite_origin_x = enem->width;
            enem->dir = 2 + (rand() % 2);
            if((lvl->level == 4 || lvl->level == 7)
                && enem->spawn_point == 1){
                enem->dir = 1;
            }
            if(lvl->level == 6 && enem->spawn_point == 1){
                enem->dir = 1;
            }
        break;
    }
}

void ChooseEnemColor(EntityInfo* enem){
    switch(enem->lives){
        case 0:
            esat::DrawSprite(*(enem->color.colors + 0), enem->pos);
        break;
        case 1:
            esat::DrawSprite(*(enem->color.colors + 1), enem->pos);
        break;
        case 2:
            esat::DrawSprite(*(enem->color.colors + 2), enem->pos);
        break;
        case 3:
            esat::DrawSprite(*(enem->color.colors + 3), enem->pos);
        break;
        case 4:
            esat::DrawSprite(*(enem->color.colors + 4), enem->pos);
        break;
        case 5:
            esat::DrawSprite(*(enem->color.colors + 5), enem->pos);
        break;
        case 6:
            esat::DrawSprite(*(enem->color.colors + 6), enem->pos);
        break;
    }
}

void StalkingPlayer(EntityInfo* plyr, EntityInfo* enem, LevelHandling* lvl){
    if((lvl->level == 7 || lvl->level == 4) && enem->inside){
        esat::Vec2 v = { {plyr->pos.x - enem->pos.x},
                         {plyr->pos.y - enem->pos.y} };
        v = Vec2Normalization(v);
        v = Vec2MultScale(v, 0.1f);
        enem->pos.x = enem->pos.x + v.x * cosf(enem->angle);
        enem->pos.y = enem->pos.y + v.y * sinf(enem->angle);
    }
}

void ImplementInArray(EntityInfo* plyr, EntityInfo* enem, LevelHandling* lvl){
    for(int i = 0; i < kNEnemAmount; i++){
        for(int j = 0; j < kNPlayerAmount; j++){
            StalkingPlayer((plyr + j), (enem + i), lvl);
        }
    }
}

void BouncingNotLost(EntityInfo* enem, LevelHandling* lvl){
    if((lvl->level == 1 || lvl->level == 2 || lvl->level == 5
        || lvl->level == 4 || lvl->level == 7)){
        if(enem->spawn_point == 1
        && enem->pos.x > kNWindowWidth
        && enem->inside){
            enem->pos.x -= kNWindowWidth;
        } else {
            if(enem->pos.x < 0 
                && enem->spawn_point == 0
                && enem->inside){
                enem->pos.x += kNWindowWidth;
            }
        }
        if(!enem->inside
            && enem->pos.x > 0
            && enem->spawn_point == 0){
                enem->inside = true;
        }
        if(!enem->inside
            && enem->pos.x < kNWindowWidth
            && enem->spawn_point == 1){
                enem->inside = true;
        }
    }
}

void ReviveDeadEnemies(EntityInfo* enem, LevelHandling* lvl, double dt){
    if(!enem->alive){
        enem->rev_timer += dt * 0.001;
    }
    if(enem->rev_timer >= 0.5 && !is_level_changing){
        enem->rev_timer -= enem->rev_timer;
        SetEnemyInfo(enem);
        SpawnPoint(enem, lvl);
    }
}