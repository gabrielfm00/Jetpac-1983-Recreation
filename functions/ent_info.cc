#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

#define M_PI 3.14159265358979323846

EntityInfo* InitiateEntities(){

    return (EntityInfo*)malloc(20 * sizeof(EntityInfo));
}

esat::Vec2* InitiatePoints(){

    return (esat::Vec2*)malloc(5 * sizeof(esat::Vec2));
}

void ShapeMaker(EntityInfo* rectangle, esat::Vec2* points){
    float width = rectangle->width;
    float height = rectangle->height;

    (points + 0)->x = rectangle->pos.x;
    (points + 0)->y = rectangle->pos.y;
    (points + 1)->x = rectangle->pos.x + width;
    (points + 1)->y = rectangle->pos.y;
    (points + 2)->x = rectangle->pos.x + width;
    (points + 2)->y = rectangle->pos.y + height;
    (points + 3)->x = rectangle->pos.x;
    (points + 3)->y = rectangle->pos.y + height;
    *(points + 4) = *(points + 0);
}

void SetPlayerInfo(EntityInfo* plyr){
    for(int i = 0; i < kNPlayerAmount; i++){
        esat::SpriteTransformInit(&(plyr + i)->pos);
        (plyr + 0)->pos = { (kNWindowWidth / 2), 510.0f, 
                            0.0f, -1.0f, 1.0f, 32.0f, 0.0f }; // body
        (plyr + 1)->pos = { (plyr + 0)->pos.x, (plyr + 0)->pos.y - 32,
                            0.0f, -1.0f, 1.0f, 32.0f, 0.0f }; // head
        (plyr + i)->width = 32.0f;
        (plyr + i)->height = 32.0f;
        (plyr + i)->alive = true;
        (plyr + i)->shooting = false;
        (plyr + i)->velocity.x = 5.0f;
        (plyr + i)->velocity.y = 12.0f;
        (plyr + i)->sprite = 0;
        (plyr + i)->id = 0;
        (plyr + i)->cooldown = 16.5f;
        (plyr + i)->rev_timer = 0.0;
    }
}

void SetShipInfo(EntityInfo* ship){
    for(int i = 0; i < kNShipAmount; i++){
        esat::SpriteTransformInit(&(ship + i)->pos);
        (ship + 0)->pos = { 520.0f, 530.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
        (ship + 1)->pos = { (kNWindowWidth / 2), 50.0f, 0.0f, 1.0f, 1.0f, 0.0f };
        (ship + 2)->pos = { 150.0f, 50.0f, 0.0f, 1.0f, 1.0f, 0.0f };
        /*(ship + i)->pos.scale_x = 1.5f;
        (ship + i)->pos.scale_y = 1.5f;*/
        (ship + i)->width = 32.0f;
        (ship + i)->height = 32.0f;
        (ship + i)->velocity.y = 5.0f;
        (ship + i)->score = 100;
        (ship + i)->alive = true;
        (ship + i)->id = 0;
        (ship + i)->assembled = false;
        (ship + i)->start_going_up = false;
    }
}

void SetBulletInfo(EntityInfo* bullt){
    for(int i = 0; i < kNBulletAmount; i++){
        esat::SpriteTransformInit(&(bullt + i)->pos);
        (bullt + i)->pos.x = -kNWindowWidth;
        (bullt + i)->pos.y = -kNWindowHeight;
        (bullt + i)->velocity.x = 15.0f;
        (bullt + i)->height = 2.0f;
        (bullt + i)->width = 20.0f;
        (bullt + 0)->width = 30.0f;
        /*(bullt + kNBulletAmount)->width = 300.0f;*/
        (bullt + i)->cooldown = 0.1f;
    }
}

void SetColShapeInfo(EntityInfo* bec, EntityInfo* ex_points){
    /*Information for the Beacon*/
    esat::SpriteTransformInit(&bec->pos);
    bec->pos = { 520.0f, 0.0f };
    bec->width = 32.0f;
    bec->height = kNWindowHeight;

    /*Information for the other pickables*/
    esat::SpriteTransformInit(&ex_points->pos);
    ex_points->pos = {(float)(32 + rand() % (kNWindowWidth - 96)), 60.0f,
                       0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    ex_points->width = 32.0f;
    ex_points->height = 32.0f;
    ex_points->velocity.y = 5.0f;
    ex_points->score = 250;
    ex_points->alive = false;
    ex_points->sprite = rand() % 4;
    ex_points->lives = rand() % 6;
    ex_points->spawn_point = 3 + rand() % 10;
}   

void SetFuelInfo(EntityInfo* flcl){
    /*Information for the Fuel*/
    esat::SpriteTransformInit(&flcl->pos);
    flcl->pos = {(float)(32 + rand() % (kNWindowWidth - 96)), 60.0f,
                 0.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    flcl->width = 32.0f;
    flcl->height = 32.0f;
    flcl->velocity.y = 5.0f;
    flcl->alive = false;
    flcl->score = 100;
    flcl->fuel_count = 0;
    flcl->shooting = false;
}

void Leveling(LevelHandling* lvl){
    lvl->level = 0;
    lvl->ship_level = 0;
}

void SetEnemyInfo(EntityInfo* enem){
    esat::SpriteTransformInit(&enem->pos);

    enem->angle = 45.0f * M_PI / 180.0f;
    enem->velocity = { 5.0f, 6.0f };
    enem->sprite = 0;
    enem->spawn_point = rand() % 2;
    enem->lives = rand() % 6; // for color management (temporarily ? )
    enem->width = 32.0f;
    enem->height = 32.0f;
    enem->alive = true;
    enem->inside = false;
}

void SetFloorInfo(EntityInfo* flrs){
    for(int i = 0; i < kNFloorAmount; i++){
        esat::SpriteTransformInit(&(flrs + i)->pos);

        (flrs + 0)->width = 140.0f; (flrs + 0)->height = 31.0f;
        (flrs + 1)->width = 90.0f; (flrs + 1)->height = 30.0f;
        (flrs + 2)->width = 140.0f; (flrs + 2)->height = 30.0f;
        (flrs + 3)->width = (kNWindowWidth + 100); (flrs + 3)->height = 27.0f;
        (flrs + 4)->width = (kNWindowWidth + 100); (flrs + 4)->height = 10.0f; 
        (flrs + 0)->pos = { 102.0f, 219.0f }; (flrs + 1)->pos = { 375.0f, 293.0f };
        (flrs + 2)->pos = { 597.0f, 147.0f }; (flrs + 3)->pos = { 0.0f, 565.0f };
        (flrs + 4)->pos = { 0.0f, 30.0f };
    }
}

void SetSideTerrainInfo(EntityInfo* sd_terrain){
    for(int i = 0; i < 6; i++){
        esat::SpriteTransformInit(&(sd_terrain + i)->pos);

        (sd_terrain + 0)->width = 5.0f; (sd_terrain + 0)->height = 29.0f;
        (sd_terrain + 1)->width = 5.0f; (sd_terrain + 1)->height = 29.0f;
        (sd_terrain + 2)->width = 5.0f; (sd_terrain + 2)->height = 28.0f;
        (sd_terrain + 3)->width = 5.0f; (sd_terrain + 3)->height = 28.0f;
        (sd_terrain + 4)->width = 5.0f; (sd_terrain + 4)->height = 28.0f;
        (sd_terrain + 5)->width = 5.0f; (sd_terrain + 5)->height = 28.0f;
        (sd_terrain + 0)->pos = { 97.0f, 220.0f }; (sd_terrain + 1)->pos = { 243.0f, 220.0f };
        (sd_terrain + 2)->pos = { 370.0f, 295.0f }; (sd_terrain + 3)->pos = { 465.0f, 295.0f };
        (sd_terrain + 4)->pos = { 593.0f, 149.0f }; (sd_terrain + 5)->pos = { 737.0f, 149.0f };
    }
}

void SetMainMenuInfo(EntityInfo* mm){
    for(int i = 0; i < 4; i++){
        esat::SpriteTransformInit(&(mm + i)->pos);

        (mm + 0)->pos = { 140.0f, 182.0f };
        (mm + 1)->pos = { 140.0f, 233.0f };
        (mm + 2)->pos = { 140.0f, 281.0f };
        (mm + 3)->pos = { 140.0f, 384.0f };
        (mm + i)->width = 470.0f;
        (mm + i)->height = 30.0f;
        (mm + i)->control.plyr_1 = { 0, 0, 0 }; 
        (mm + i)->control.plyr_2 = { 0, 0, 0 };
        (mm + i)->control.start_game = { 0, 0, 0 };
        (mm + i)->control.others = { 0, 0, 0 };

    }
}

void EntitiesInMemory(){
    player = InitiateEntities();
    player_2 = InitiateEntities();
    bullets = (EntityInfo*)calloc(kNBulletAmount, sizeof(EntityInfo));
    ground = InitiateEntities();
    foes = (EntityInfo*)calloc(kNEnemAmount, sizeof(EntityInfo));
    air_craft = InitiateEntities();
    air_craft_2 = InitiateEntities();
    obstacles = InitiateEntities();
    side_terrain = InitiateEntities();
    menu_options = InitiateEntities();
}

void FreeEntities(){
    free(player); player = nullptr;
    free(player_2); player_2 = nullptr;
    free(bullets); bullets = nullptr;
    free(ground); ground = nullptr;
    free(foes); foes = nullptr;
    free(air_craft); air_craft = nullptr;
    free(air_craft_2); air_craft_2 = nullptr;
    free(obstacles); obstacles = nullptr;
    free(side_terrain); side_terrain = nullptr;
    free(menu_options); menu_options = nullptr;
}