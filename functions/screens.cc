#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

void SetColors(MenuOptionColors* cc){
    cc->plyr_1.r = 255;
    cc->plyr_1.g = 255;
    cc->plyr_1.b = 255;
    cc->plyr_2.r = 255;
    cc->plyr_2.g = 255;
    cc->plyr_2.b = 255;
    cc->start_game.r = 255;
    cc->start_game.g = 255;
    cc->start_game.b = 255;
    cc->others.r = 255;
    cc->others.g = 255;
    cc->others.b = 255;
}

void PlaceHolderUI(long long int score, long long int h_score, long long int p2_score){
    g_scores = (char *)malloc(40 * sizeof(char));
    esat::DrawSetTextBlur(0);
    esat::DrawSetTextFont(kNFontPath);
    esat::DrawSetTextSize(24);
    esat::DrawSetFillColor(255, 255, 255);
    esat::DrawText(75, 21, "1UP");
    esat::DrawText(669, 21, "2UP");
    esat::DrawSetFillColor(0, 255, 255);
    esat::DrawText(371, 21, "HI");
    esat::DrawSetFillColor(255, 255, 0);
    esat::DrawSetTextSize(kNGameFontSize);
    itoa(score, g_scores, 10);
    sprintf(g_scores, "%06lld", score);
    esat::DrawText(35, 50, g_scores);
    itoa(score, g_scores, 10);
    sprintf(g_scores, "%06lld", p2_score);
    esat::DrawText(627, 50, g_scores);
    itoa(h_score, g_scores, 10);
    sprintf(g_scores, "%06lld", h_score);
    esat::DrawText((kNWindowWidth - 130) / 2, 50, g_scores);
    esat::DrawSetFillColor(255, 255, 255);
}

void InGameUI(long long int score, long long int h_score, long long int p2_score,
              int p1_lives, int p2_lives = NULL){
  g_scores = (char*)malloc(40 * sizeof(char));
  esat::DrawSetTextBlur(0);
  esat::DrawSetTextFont(kNFontPath);
  esat::DrawSetTextSize(24);
  esat::DrawSetFillColor(255, 255, 0);
  esat::DrawSetTextSize(kNGameFontSize);
  itoa(score, g_scores, 10);
  sprintf(g_scores, "%06lld", score);
  esat::DrawText(35, 50, g_scores);
  itoa(score, g_scores, 10);
  sprintf(g_scores, "%06lld", p2_score);
  esat::DrawText(627, 50, g_scores);
  itoa(h_score, g_scores, 10);
  sprintf(g_scores, "%06lld", h_score);
  esat::DrawText((kNWindowWidth - 130) / 2, 50, g_scores);
  esat::DrawSetFillColor(255, 255, 255);
  itoa(p1_lives, g_scores, 10);
  sprintf(g_scores, "%01d", p1_lives);
  esat::DrawText(200, 21, g_scores);
  itoa(p1_lives, g_scores, 10);
  sprintf(g_scores, "%01d", p2_lives);
  esat::DrawText(550, 21, g_scores);
}

void MainMenu(MenuOptionColors* cc, EntityInfo* mm){
    esat::Vec2 point = { (float)esat::MousePositionX(), 
                         (float)esat::MousePositionY() };
    for(int i = 0; i < 4; i++){
        esat::Vec2* mm_points;
        mm_points = InitiatePoints();
        ShapeMaker((mm + i), mm_points);
        switch(i){
            case 0:
                esat::DrawSetFillColor(mm->control.plyr_1.r, mm->control.plyr_1.g, mm->control.plyr_1.b);
                esat::DrawSetStrokeColor(mm->control.plyr_1.r, mm->control.plyr_1.g, mm->control.plyr_1.b);
            break;
            case 1:
                esat::DrawSetFillColor(mm->control.plyr_2.r, mm->control.plyr_2.g, mm->control.plyr_2.b);
                esat::DrawSetStrokeColor(mm->control.plyr_2.r, mm->control.plyr_2.g, mm->control.plyr_2.b);
            break;
            case 2:
                esat::DrawSetFillColor(mm->control.others.r, mm->control.others.g, mm->control.others.b);
                esat::DrawSetStrokeColor(mm->control.others.r, mm->control.others.g, mm->control.others.b);
            break;
            case 3:
                esat::DrawSetFillColor(mm->control.start_game.r, mm->control.start_game.g, mm->control.start_game.b);
                esat::DrawSetStrokeColor(mm->control.start_game.r, mm->control.start_game.g, mm->control.start_game.b);
            break;
        }
        esat::DrawSolidPath((float*)mm_points, 5);
    }

    esat::DrawSetTextBlur(0);
    esat::DrawSetTextSize(22);
    esat::DrawSetTextFont(kNFontPath);
    esat::DrawSetFillColor(255, 255, 255);
    esat::DrawText(148, 123, "JETPAC GAME SELECTION");
    esat::DrawSetFillColor(cc->plyr_1.r, cc->plyr_1.g, cc->plyr_1.b);
    esat::DrawText(148, 205, "1    1 PLAYER GAME");
    esat::DrawSetFillColor(cc->plyr_2.r, cc->plyr_2.g, cc->plyr_2.b);
    esat::DrawText(148, 255, "2    2 PLAYER GAME");
    esat::DrawSetFillColor(cc->others.r, cc->others.g, cc->others.b);
    esat::DrawText(148, 305, "3    3 CREDITS");
    esat::DrawSetFillColor(cc->start_game.r, cc->start_game.g, cc->start_game.b);
    esat::DrawText(148, 405, "5    5 START GAME");
    esat::DrawSetFillColor(255, 255, 255);
    esat::DrawText(50, 550, "©1983 A.C.G ALL RIGHTS RESERVED");
}

void ToggleOptions(MenuOptionColors* cc, EntityInfo* mm){
    if(g_blink_options_control != 0){
        mm->control.plyr_1 = { 0, 0, 0 };
        cc->plyr_1.r = 255;
        cc->plyr_1.g = 255;
        cc->plyr_1.b = 255;
    }
    if(g_blink_options_control != 1){
        mm->control.plyr_2 = { 0, 0, 0 };
        cc->plyr_2.r = 255;
        cc->plyr_2.g = 255;
        cc->plyr_2.b = 255;
    }
    if(g_blink_options_control != 2){
        mm->control.others = { 0, 0, 0 };
        cc->others.r = 255;
        cc->others.g = 255;
        cc->others.b = 255;
    }
}

void GiveInfoToPlayer(EntityInfo* plyr, EntityInfo* plyr_2){
    if(esat::IsKeyDown('1')){
        g_blink_options_control = 0;
    } else {
        if(esat::IsKeyDown('2')){
            g_blink_options_control = 1;
        }
    }
    if(esat::IsKeyDown('3')){
        g_blink_options_control = 2;
    }
    if(esat::IsKeyDown('5') && g_blink_options_control == 0){
        change_screens = one_player_game;
        plyr->lives = 5;
        plyr->score = 0;
        plyr->alive = true;
    }
    if(esat::IsKeyDown('5') && g_blink_options_control == 1){
        change_screens = two_player_game;
        plyr->lives = 4;
        plyr->score = 0;
        plyr->alive = true;
        plyr_2->lives = 5;
        plyr_2->score = 0;
    }
}

void BlinkingMainMenu(MenuOptionColors* cc, EntityInfo* mm, 
                      double dt, unsigned char fps){

    switch(g_blink_options_control){
        case 0:
            g_menu_time += dt * 0.001;
            if(g_menu_time <= 1.0){
                mm->control.plyr_1 = { 255, 255, 255 }; 
                cc->plyr_1.r = 0;
                cc->plyr_1.g = 0;
                cc->plyr_1.b = 0;
            } else {
                if(g_frames%(fps + 10) == 0){
                    g_menu_time -= g_menu_time;
                }
                mm->control.plyr_1 = { 0, 0, 0 };
                cc->plyr_1.r = 255;
                cc->plyr_1.g = 255;
                cc->plyr_1.b = 255;
            }
        break;
        case 1:
            g_menu_time += dt * 0.001;
            if(g_menu_time <= 1.0){
                mm->control.plyr_2 = { 255, 255, 255 }; 
                cc->plyr_2.r = 0;
                cc->plyr_2.g = 0;
                cc->plyr_2.b = 0;
            } else {
                if(g_frames%(fps + 10) == 0){
                    g_menu_time -= g_menu_time;
                }
                mm->control.plyr_2 = { 0, 0, 0 };
                cc->plyr_2.r = 255;
                cc->plyr_2.g = 255;
                cc->plyr_2.b = 255;
            }
        break;
        case 2:
            g_menu_time += dt * 0.001;
            if(g_menu_time <= 1.0){
                mm->control.others = { 255, 255, 255 }; 
                cc->others.r = 0;
                cc->others.g = 0;
                cc->others.b = 0;
            } else {
                if(g_frames%(fps + 10) == 0){
                    g_menu_time -= g_menu_time;
                }
                mm->control.others = { 0, 0, 0 };
                cc->others.r = 255;
                cc->others.g = 255;
                cc->others.b = 255;
            }
        break;
    }
    switch(g_start_game){
        case 0:
            g_start_game_time += dt * 0.001;
            if(g_start_game_time <= 1.0){
                mm->control.start_game = { 255, 255, 255 }; 
                cc->start_game.r = 0;
                cc->start_game.g = 0;
                cc->start_game.b = 0;
            } else {
                if(g_frames%(fps + 10) == 0){
                    g_start_game_time -= g_start_game_time;
                }
                mm->control.start_game = { 0, 0, 0 };
                cc->start_game.r = 255;
                cc->start_game.g = 255;
                cc->start_game.b = 255;
            }
        break;
    }
}

void GameOverScreen(){
    switch(g_possible_over){
        case 0:
            esat::DrawSetTextBlur(0);
            esat::DrawSetTextSize(22);
            esat::DrawSetTextFont(kNFontPath);
            esat::DrawSetFillColor(255, 255, 255);
            esat::DrawText(200, (kNWindowHeight / 2), "GAME OVER PLAYER 1");
        break;
        case 1:
            esat::DrawSetTextBlur(0);
            esat::DrawSetTextSize(22);
            esat::DrawSetTextFont(kNFontPath);
            esat::DrawSetFillColor(255, 255, 255);
            esat::DrawText(150, (kNWindowHeight / 2), "GAME OVER BOTH PLAYERS");
        break;
    }
}

void BootUpGameOver(EntityInfo* plyr, EntityInfo* plyr_2, 
                    EntityInfo* a_craft, EntityInfo* a_craft_2,
                    EntityInfo* flcl, EntityInfo* flcl_2,
                    LevelHandling* lvl, LevelHandling* lvl_2, 
                    EntityInfo* enem, EntityInfo* ex_points, double dt){
    if(plyr->lives <= 0 && change_screens != main_menu
        && change_screens == one_player_game){
        change_screens = game_over;
        g_possible_over = 0;
    }
    if(plyr_2->lives <= 0 && change_screens != main_menu
        && change_screens == two_player_game){
        change_screens = game_over;
        g_possible_over = 1;
    }
    if(plyr->lives <= 0){
        plyr->alive = false;
    }
    if(plyr_2->lives <= 0){
        plyr_2->alive = false;
    }
    if(change_screens == game_over){
        g_over_timer += dt * 0.001;
    }
    if(g_over_timer >= 2.5){
        g_over_timer -= g_over_timer;
        change_screens = main_menu;
        Leveling(lvl);
        Leveling(lvl_2);
        SetPlayerInfo(plyr);
        SetPlayerInfo(plyr_2);
        SetShipInfo(a_craft);
        SetShipInfo(a_craft_2);
        SetFuelInfo(flcl);
        SetFuelInfo(flcl_2);
        SetColShapeInfo(&beacon, ex_points);
        for(int i = 0; i < kNEnemAmount; i++){
            SetEnemyInfo((enem + i));
        }
    }
}