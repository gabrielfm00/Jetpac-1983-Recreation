#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

void FueLogic(EntityInfo* flcl, esat::SpriteHandle* pickable, unsigned char fps){
  if(!flcl->alive && flcl->shooting 
  && g_timer_alive%(fps + 35) == 0
  && flcl->fuel_count < 6
  && !is_level_changing){
    flcl->alive = true;
  }
  if(flcl->alive){
    esat::DrawSprite(*(flcl->color.colors + 2), flcl->pos);
    esat::DrawSprite(*(pickable + 5), flcl->pos);
    flcl->pos.y += flcl->velocity.y;
  }
  if(flcl->pos.y >= kNWindowHeight){
    flcl->alive = false;
  }
}

void PickableLogic(EntityInfo* ex_points, esat::SpriteHandle* pickable, double dt){
  if(!ex_points->alive){
    g_pickable_timer += dt * 0.001;
  }
  if(!ex_points->alive 
    && g_pickable_timer >= ex_points->spawn_point
    && !is_level_changing){
    g_pickable_timer -= g_pickable_timer;
    ex_points->alive = true;
  }
  if(ex_points->sprite == 1){
    ++ex_points->lives %= 6;
  }
  if(ex_points->alive){
    esat::DrawSprite(*(ex_points->color.colors + ex_points->lives), ex_points->pos);
    esat::DrawSprite(*(pickable + ex_points->sprite), ex_points->pos);
    ex_points->pos.y += ex_points->velocity.y;
  }
}

void ShipFuelFillUp(EntityInfo* a_craft, EntityInfo* flcl){
    if(flcl->fuel_count >= 1){
      esat::DrawSprite(*((a_craft + 0)->color.colors + 7), 
                      (a_craft + 0)->pos.x, 
                      (a_craft + 0)->pos.y + 16);
    }
    if(flcl->fuel_count >= 2){
      esat::DrawSprite(*((a_craft + 0)->color.colors + 7), 
                      (a_craft + 0)->pos.x, 
                      (a_craft + 0)->pos.y);
    }
    if(flcl->fuel_count >= 3){
      esat::DrawSprite(*((a_craft + 1)->color.colors + 7), 
                      (a_craft + 1)->pos.x, 
                      (a_craft + 1)->pos.y + 16);
    }
    if(flcl->fuel_count >= 4){
      esat::DrawSprite(*((a_craft + 1)->color.colors + 7),
                       (a_craft + 1)->pos.x,
                       (a_craft + 1)->pos.y);
    }
    if(flcl->fuel_count >= 5){
      esat::DrawSprite(*((a_craft + 2)->color.colors + 7),
                       (a_craft + 2)->pos.x,
                       (a_craft + 2)->pos.y + 16);
    }
    if(flcl->fuel_count >= 6){
      esat::DrawSprite(*((a_craft + 2)->color.colors + 7),
                       (a_craft + 2)->pos.x,
                       (a_craft + 2)->pos.y);
    }
}

void DrawSpaceShip(esat::SpriteHandle* ship, EntityInfo* a_craft, 
                  esat::SpriteHandle* comb, EntityInfo* flcl, unsigned char fps){
  esat::DrawSprite(*((a_craft + 0)->color.colors + 6), (a_craft + 0)->pos);
  esat::DrawSprite(*((a_craft + 1)->color.colors + 6), (a_craft + 1)->pos);
  esat::DrawSprite(*((a_craft + 2)->color.colors + 6), (a_craft + 2)->pos);
  ShipFuelFillUp(a_craft, flcl);
  esat::DrawSprite(*(ships + 2), (a_craft + 0)->pos);
  esat::DrawSprite(*(ships + 1), (a_craft + 1)->pos);
  esat::DrawSprite(*(ships + 0), (a_craft + 2)->pos);

  if(a_craft->start_going_up){
    esat::DrawSprite(*((a_craft + 0)->color.colors + 1), 
                    (a_craft + 0)->pos.x, 
                    (a_craft + 0)->pos.y + 32);
    esat::DrawSprite(*(comb + g_combustion_frames),
                     (a_craft + 0)->pos.x,
                     (a_craft + 0)->pos.y + 32);
    if(g_frames%(fps / 10) == 0){
      ++g_combustion_frames %= 2;
    }
  }
}

void ChangeShipLevel(EntityInfo* a_craft, LevelHandling* lvl, double dt){
  if(lvl->ship_level == 0 && a_craft->id <= 0){
    g_estoy_cansado_jefe += dt * 0.001;
    if (g_estoy_cansado_jefe >= 0.4){
      g_estoy_cansado_jefe -= g_estoy_cansado_jefe;
      is_level_changing = false;
      a_craft->assembled = false;
      SetShipInfo(a_craft);
      a_craft->id += 1;
    }
  }
  if(lvl->ship_level == 4 && a_craft->id <= 1){
    g_estoy_cansado_jefe += dt * 0.001;
    if (g_estoy_cansado_jefe >= 0.4){
      g_estoy_cansado_jefe -= g_estoy_cansado_jefe;
      is_level_changing = false;
      a_craft->assembled = false;
      SetShipInfo(a_craft);
      a_craft->id += 1;
    }
  }
  if(lvl->ship_level == 8 && a_craft->id <= 2){
    g_estoy_cansado_jefe += dt * 0.001;
    if (g_estoy_cansado_jefe >= 0.5){
      g_estoy_cansado_jefe -= g_estoy_cansado_jefe;
      is_level_changing = false;
      a_craft->assembled = false;
      SetShipInfo(a_craft);
      a_craft->id += 1;
    }
  }
  if(lvl->ship_level == 12 && a_craft->id <= 3){
    g_estoy_cansado_jefe += dt * 0.001;
    if (g_estoy_cansado_jefe >= 0.5){
      g_estoy_cansado_jefe -= g_estoy_cansado_jefe;
      is_level_changing = false;
      a_craft->assembled = false;
      SetShipInfo(a_craft);
      a_craft->id += 1;
    }
  }
}
