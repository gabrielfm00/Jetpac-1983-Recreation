#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

const unsigned int kNWindowWidth = 800;
const unsigned int kNWindowHeight = 600;
const unsigned int kNEnemAmount = 12;
const unsigned int kNFloorAmount = 5;
const unsigned int kNTerrainAmount = 6;
const unsigned int kNBulletAmount = 12;
const unsigned int kNPlayerAmount = 2;
const unsigned int kNShipAmount = 3;
const unsigned int kNGameFontSize = 22;

const float kNScaler = 1.5f;

char* kNAtlasPath = "./sprites/Grid.png";
char* kNMapPath = "./sprites/lvl.png";
char* kNMapPath2 = "./sprites/lvl_2.png";
char* kNFontPath = "./retro_font/zx-spectrum.ttf";

enum ToggleGame{
    main_menu,
    one_player_game,
    two_player_game,
    game_over,
};

struct Color{
    esat::SpriteHandle* colors = nullptr;
};

struct MenuColors{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct MenuOptionColors{
    MenuColors plyr_1;
    MenuColors plyr_2;
    MenuColors start_game;
    MenuColors others;
};

struct LevelHandling{
    int level;
    int ship_level;
};

struct EntityInfo{
    esat::SpriteTransform pos;
    esat::Vec2 velocity; 
    Color color;
    MenuOptionColors control;
    double timer;
    double rev_timer;
    float width;
    float height;
    float angle;
    float cooldown;
    bool shooting;
    bool inside;
    bool alive;
    bool assembled;
    bool start_going_up;
    int spawn_point;
    int sprite;
    int lives; // used for color on the enemies
    int score;
    int dir;
    int id;
    int fuel_count;
};

struct InfoControl{
    EntityInfo p1;
    EntityInfo ship1;
    EntityInfo pickable1;
};

esat::SpriteHandle atlas;
esat::SpriteHandle playing_ground; 
esat::SpriteHandle playing_ground2;
esat::SpriteHandle* ships = nullptr;
esat::SpriteHandle* power_ups = nullptr;
esat::SpriteHandle* enemies = nullptr;
esat::SpriteHandle* head = nullptr;
esat::SpriteHandle* body = nullptr;
esat::SpriteHandle* explosion = nullptr;
esat::SpriteHandle* combustion = nullptr;

EntityInfo* player = nullptr; // memory block for the player.
EntityInfo* player_2 = nullptr;
EntityInfo* bullets = nullptr; // memory block for the bullets.
EntityInfo* foes = nullptr;
EntityInfo* air_craft = nullptr;
EntityInfo* air_craft_2 = nullptr;
EntityInfo* obstacles = nullptr;
EntityInfo* ground = nullptr;
EntityInfo* side_terrain = nullptr;
EntityInfo* menu_options = nullptr;
EntityInfo extra_points;
EntityInfo beacon;
EntityInfo fuel;
EntityInfo fuel_2;
EntityInfo samia;
EntityInfo flore;

LevelHandling levels;
LevelHandling levels_2;

MenuOptionColors control_menu;

ToggleGame change_screens = main_menu;

MenuColors laser_colors;

FILE* first_player;
FILE* second_player;
FILE* h_score;

bool is_shooting = false; // checks if the player is shooting. 
bool is_level_changing = false; // checks for a level change.
bool going_down = true; // check if the player is affected by gravity.
bool bounce_off = false; // check if it can go up.
bool go_left = true; // check if it can go left.
bool go_right = true; // check if it can go right.
bool p1_turn = true;
bool p2_turn = false;

long long int high_score = 0;
long long int s4mia = 0;

int g_timer_alive = 0; // timer to revive an enemy that is dead. 
int g_frames = 0; // control for frames of animation.
int g_combustion_frames = 0; // frames of animation of the combustion.
int g_explosion_frames = 0; // frames of the explosion sprites.
int g_gravity = 5; // self explanatory.
int g_ship_gravity = 4; // test something quickly.
int g_current_shot = 0; // counter of the current shot. 
int g_head_sprite = 0; // frame for the player head sprite.
int g_fuel_counter = 0; // count quantity of fuel on screen.
int g_fuel_full = 0; // count if the ship is full.
int g_pickable_counter = 0; // count quantity of items on screen;
int g_blink_options_control = 0;
int g_start_game = 0;
int g_state_of_game = 0;
int g_possible_over = 1;
int g_color_of_bullet = 0;

char* g_scores = "\0";

double g_elapsed_shot_time = 0.0; // elapsed time for the player shot.
double g_elapsed_level_change = 0.0; // elapsed time for the level change. 
double g_menu_time = 0.0;
double g_start_game_time = 0.0;
double g_revive_time = 0.0;
double g_estoy_cansado_jefe = 0.0;
double g_pickable_timer = 0.0;
double g_over_timer = 0.0;
double g_player_timer = 0.0;
double g_player_timer_2 = 0.0;