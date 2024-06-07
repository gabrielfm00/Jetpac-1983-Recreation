#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

#include "./functions/variables.cc"
#include "./functions/ent_info.cc"
#include "./functions/collisions.cc"
#include "./functions/player_behavior.cc"
#include "./functions/enem_behavior.cc"
#include "./functions/sub_sprites.cc"
#include "./functions/space_ship.cc"
#include "./functions/screens.cc"
#include "./save_info.cc"

#define NDEBUG

#ifndef NDEBUG

void DebuggingHitBox(EntityInfo* enem, EntityInfo* flrs, 
                    EntityInfo* plyr, EntityInfo* a_craft,
                    EntityInfo* flcl, EntityInfo* sd_terrain){
  esat::Vec2 *points;
  points = InitiatePoints();
  ShapeMaker(enem, points);
  esat::DrawSetStrokeColor(245, 200, 150);
  esat::DrawSolidPath((float*)points, 5);

  for(int i = 0; i < kNFloorAmount; i++){
    esat::Vec2* flrs_points;
    flrs_points = InitiatePoints();
    ShapeMaker((flrs + i), flrs_points);
    esat::DrawSetStrokeColor(0, 255, 255);
    esat::DrawSetFillColor(0, 0, 0, 0);
    esat::DrawSolidPath((float*)flrs_points, 5);
  }

  for(int i = 0; i < kNPlayerAmount; i++){
    esat::Vec2* plyr_points;
    plyr_points = InitiatePoints();
    ShapeMaker((plyr + i), plyr_points);
    esat::DrawSetStrokeColor(0, 0, 255);
    esat::DrawSetFillColor(0, 0, 0, 0);
    esat::DrawSolidPath((float*)plyr_points, 5);
  }

  esat::Vec2* bec_points;
  bec_points = InitiatePoints();
  ShapeMaker(&beacon, bec_points);
  esat::DrawSetStrokeColor(255, 0, 255);
  esat::DrawSetFillColor(0, 0, 0, 0);
  esat::DrawSolidPath((float *)bec_points, 5);

  for(int i = 0; i < kNShipAmount; i++){
    esat::Vec2* ship_points;
    ship_points = InitiatePoints();
    ShapeMaker((a_craft + i), ship_points);
    esat::DrawSetStrokeColor(159, 2, 81);
    esat::DrawSetFillColor(0, 0, 0, 0);
    esat::DrawSolidPath((float*)ship_points, 5);
  }

  esat::Vec2* fuel_points;
  fuel_points = InitiatePoints();
  ShapeMaker(flcl, fuel_points);
  esat::DrawSetStrokeColor(51, 255, 51);
  esat::DrawSetFillColor(0, 0, 0, 0);
  esat::DrawSolidPath((float*)fuel_points, 5);

  for(int i = 0; i < 6; i++){
    esat::Vec2* sd_points;
    sd_points = InitiatePoints();
    ShapeMaker((sd_terrain + i), sd_points);
    esat::DrawSetStrokeColor(159, 2, 81);
    esat::DrawSetFillColor(0, 0, 0, 0);
    esat::DrawSolidPath((float *)sd_points, 5);
  }
}

#endif

void DrawPlayer(EntityInfo* plyr, int num){
  if(plyr->alive){
    esat::DrawSprite(*((plyr + 0)->color.colors + num), (plyr + 0)->pos);
    esat::DrawSprite(*(body + plyr->sprite), (plyr + 0)->pos);
    esat::DrawSprite(*((plyr + 0)->color.colors + num), (plyr + 1)->pos);
    esat::DrawSprite(*(head + g_head_sprite), (plyr + 1)->pos);
  }
}


void FreeOtherPointers(){
  free(g_scores); g_scores = nullptr;
  free(kNAtlasPath); kNAtlasPath = nullptr;
  free(kNFontPath); kNFontPath = nullptr;
  free(kNMapPath); kNMapPath = nullptr;
}

int esat::main(int argc, char** argv) {
  srand(time(nullptr));
  double current_time = 0.0;
  double last_time = 0.0;
  double fps = 25.0;
  esat::WindowInit(kNWindowWidth, kNWindowHeight);

  WindowSetMouseVisibility(true);

  double dt = 0.0;

  atlas = esat::SpriteFromFile(kNAtlasPath); 
  playing_ground = esat::SpriteFromFile(kNMapPath);
  playing_ground2 = esat::SpriteFromFile(kNMapPath2);

  SpritesInMemory();
  EntitiesInMemory();
  
  LoadScores();

  player->score = 0;
  player_2->score = 0;
  player->lives = 5;
  player_2->lives = 4;

  SetColors(&control_menu);
  PowerSubSprites(atlas, power_ups);
  ExplosionSubSprites(atlas, explosion);
  HeadSubSprites(atlas, head);
  BodySubSprites(atlas, body);
  CombustionSubSprites(atlas, combustion);
  Leveling(&levels);
  Leveling(&levels_2);
  SetBulletInfo(bullets);
  SetPlayerInfo(player);
  SetPlayerInfo(player_2);
  SetFloorInfo(ground);
  SetShipInfo(air_craft);
  SetShipInfo(air_craft_2);
  SetFuelInfo(&fuel);
  SetFuelInfo(&fuel_2);
  SetColShapeInfo(&beacon, &extra_points);
  SetSideTerrainInfo(side_terrain);
  SetMainMenuInfo(menu_options);
  ColorSubSprites(atlas, &fuel);
  ColorSubSprites(atlas, &fuel_2);
  ColorSubSprites(atlas, &extra_points);
  for(int i = 0; i < kNPlayerAmount; i++){
    ColorSubSprites(atlas, (player + i));
    ColorSubSprites(atlas, (player_2 + i));
  }
  for(int s = 0; s < kNShipAmount; s++){
    ColorSubSprites(atlas, (air_craft + s));
    ColorSubSprites(atlas, (air_craft_2 + s));
  }
  for(int i = 0; i < kNEnemAmount; i++){
    ColorSubSprites(atlas, (foes + i));
    SetEnemyInfo((foes + i));
    SpawnPoint((foes + i), &levels);
    SpawnPoint((foes + i), &levels_2);
  }

  while (!esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) &&
         esat::WindowIsOpened()) {
    dt = esat::Time() - last_time;
    last_time = esat::Time();
    g_timer_alive++;
    g_frames++;
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);

    SaveScores();
    BootUpGameOver(player, player_2, 
                  air_craft, air_craft_2,
                  &fuel, &fuel_2,
                  &levels, &levels_2,
                  foes, &extra_points, dt);
    SelectBulletColor(&laser_colors);
    ResetLevels(&levels, &levels_2,
                air_craft, air_craft_2);

    if(player->score >= high_score){ high_score = player->score; }
    if(player_2->score >= high_score){ high_score = player_2->score; }

    switch(change_screens){
      case 0:
        ToggleOptions(&control_menu, menu_options);
        BlinkingMainMenu(&control_menu, menu_options,
                        dt, fps);
        MainMenu(&control_menu, menu_options);
        PlaceHolderUI(0, high_score, 0);
        GiveInfoToPlayer(player, player_2);
      break;
      case 1:
        esat::DrawSprite(playing_ground, 0.0f, 0.0f);
        LaserCollideEnemy(player, foes, bullets);
        SwitchUpShips(atlas, ships, &levels);
        ShipCollideWithFloor(air_craft, ground, &beacon);
        PickableLogic(&extra_points, power_ups, dt);
        PlayerCollideEnemy(player, foes, dt, 1);
        FueLogic(&fuel, power_ups, fps);
        EnemyGroundCollision(foes, ground, side_terrain, &levels);
        ExplosionDeathAnimation(foes, explosion, fps, dt);
        PlayerFloorCollide(ground, player, side_terrain);
        MovePlayer(player, ground, fps);
        PlayerShooting(bullets, player, laser_colors, dt);
        DrawSpaceShip(ships, air_craft, 
                      combustion, &fuel, fps);
        DrawPlayer(player, 6);
        PlayerFrameControl(player, explosion, fps);
        ChangeShipLevel(air_craft, &levels, dt);
        FuelCollide(ground, &fuel, 
                        air_craft, player, &beacon);
        PicakblesCollide(player, &extra_points, ground);
        CollideShipPart(player, air_craft, &beacon, &fuel);
        LevelChange(air_craft, player, foes, ground, &fuel, &levels, dt);
        for(int i = 0; i < kNEnemAmount; i++){
          ReviveDeadEnemies((foes + i), &levels, dt);
          BouncingNotLost((foes + i), &levels);
          if((foes + i)->alive){
            ImLazy((foes + i), &levels);
            SideToSideMov((foes + i), &levels);
            DifferentStart((foes + i), &levels);
            OneMov((foes + i), &levels);
            #ifndef NDEBUG
              DebuggingHitBox((foes + i), ground, 
                              player, air_craft,
                              &fuel, side_terrain);
            #endif
            ChooseEnemColor((foes + i));
            ImplementInArray(player, foes, &levels);
            SwitchUpEnemies(atlas, enemies, 
                            (foes + i), &levels, fps);
          }
        }
      InGameUI(player->score, high_score, player_2->score,
                player->lives, NULL);
      break;
      case 2:
        esat::DrawSprite(playing_ground2, 0.0f, 0.0f);
        TwoPlayerGameLoop(player, player_2, foes, dt);
        PickableLogic(&extra_points, power_ups, dt);
        SwitchUpShips(atlas, ships, &levels);
        switch(g_state_of_game){
          case 0:
            ShipCollideWithFloor(air_craft, ground, &beacon);
            PicakblesCollide(player, &extra_points, ground);
            LaserCollideEnemy(player, foes, bullets);
            DrawSpaceShip(ships, air_craft,
                      combustion, &fuel, fps);
            FueLogic(&fuel, power_ups, fps);
            PlayerCollideEnemy(player, foes, dt, 1);
            MovePlayer(player, ground, fps);
            DrawPlayer(player, 1);
            PlayerFrameControl(player, explosion, fps);
            FuelCollide(ground, &fuel, 
                            air_craft, player, &beacon);
            PlayerFloorCollide(ground, player, side_terrain);
            LevelChange(air_craft, player, foes, ground, &fuel, &levels, dt);
            PlayerShooting(bullets, player, laser_colors, dt);
            ImplementInArray(player, foes, &levels);
            CollideShipPart(player, air_craft, &beacon, &fuel);
            ChangeShipLevel(air_craft, &levels, dt);
            EnemyGroundCollision(foes, ground, side_terrain, &levels);
            ExplosionDeathAnimation(foes, explosion, fps, dt);
            for(int i = 0; i < kNEnemAmount; i++){
              ReviveDeadEnemies((foes + i), &levels, dt);
              BouncingNotLost((foes + i), &levels);
              if((foes + i)->alive){
                ImLazy((foes + i), &levels);
                SideToSideMov((foes + i), &levels);
                DifferentStart((foes + i), &levels);
                OneMov((foes + i), &levels);
                #ifndef NDEBUG
                  DebuggingHitBox((foes + i), ground, 
                                  player, air_craft,
                                  &fuel, side_terrain);
                #endif
                ChooseEnemColor((foes + i));
                SwitchUpEnemies(atlas, enemies, 
                                (foes + i), &levels, fps);
              }
            }
          break;
          case 1:
            ShipCollideWithFloor(air_craft_2, ground, &beacon);
            DrawSpaceShip(ships, air_craft_2,
                          combustion, &fuel_2, fps);
            PicakblesCollide(player_2, &extra_points, ground);
            FueLogic(&fuel_2, power_ups, fps);
            PlayerCollideEnemy(player_2, foes, dt, 0);
            MovePlayer(player_2, ground, fps);
            DrawPlayer(player_2, 4);
            PlayerFrameControl(player_2, explosion, fps);
            FuelCollide(ground, &fuel_2, 
                            air_craft_2, player_2, &beacon);
            PlayerFloorCollide(ground, player_2, side_terrain);
            LevelChange(air_craft_2, player_2, foes, ground, &fuel_2, &levels_2, dt);
            PlayerShooting(bullets, player_2, laser_colors, dt);
            ImplementInArray(player_2, foes, &levels_2);
            CollideShipPart(player_2, air_craft_2, &beacon, &fuel_2);
            ChangeShipLevel(air_craft_2, &levels_2, dt);
            LaserCollideEnemy(player_2, foes, bullets);
            EnemyGroundCollision(foes, ground, side_terrain, &levels_2);
            ExplosionDeathAnimation(foes, explosion, fps, dt);
            for(int i = 0; i < kNEnemAmount; i++){
              ReviveDeadEnemies((foes + i), &levels_2, dt);
              BouncingNotLost((foes + i), &levels_2);
              if((foes + i)->alive){
                ImLazy((foes + i), &levels_2);
                SideToSideMov((foes + i), &levels_2);
                DifferentStart((foes + i), &levels_2);
                OneMov((foes + i), &levels_2);
                #ifndef NDEBUG
                  DebuggingHitBox((foes + i), ground, 
                                  player, air_craft,
                                  &fuel, side_terrain);
                #endif
                ChooseEnemColor((foes + i));
                SwitchUpEnemies(atlas, enemies, 
                                (foes + i), &levels_2, fps);
              }
            }
          break;
        }
        InGameUI(player->score, high_score, player_2->score,
                  player->lives, player_2->lives);
      break;
      case 3:
        GameOverScreen();
        PlaceHolderUI(player->score, high_score, player_2->score);
      break;
    }

      esat::DrawEnd();
      esat::WindowFrame();

      do
      {
        current_time = esat::Time();
    } while((current_time - last_time) <= 1000.0 / fps);
  }
  FreeEntities(); FreeSprites();
  FreeColors(player); FreeColors(foes);
  FreeColors(air_craft); FreeColors(&fuel);
  FreeColors(air_craft_2); FreeColors(player_2);
  FreeColors(&fuel_2); FreeColors(&extra_points);
  FreeOtherPointers(); ReleaseSprites();
  esat::WindowDestroy();
  return 0;
}