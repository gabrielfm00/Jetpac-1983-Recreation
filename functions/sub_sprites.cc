#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

esat::SpriteHandle* InitiateSprites(){

    return (esat::SpriteHandle*)malloc(15 * sizeof(esat::SpriteHandle));
}

void CloudSubSprites(esat::SpriteHandle p_atlas, esat::SpriteHandle* cloud){
    *(cloud + 0) = esat::SubSprite(p_atlas, 198, 33, 32, 32);
    *(cloud + 1) = esat::SubSprite(p_atlas, 231, 33, 32, 32);
}

void BubbleSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* bubble){
    *(bubble + 0) = esat::SubSprite(atlas, 198, 66, 32, 32);
    *(bubble + 1) = esat::SubSprite(atlas, 231, 66, 32, 32);
}

void MeteoriteSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* meteorite){
    *(meteorite + 0) = esat::SubSprite(atlas, 198, 0, 32, 32);
    *(meteorite + 1) = esat::SubSprite(atlas, 231, 0, 32, 32);
}

void JetSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* jets){
    *(jets + 0) = esat::SubSprite(atlas, 198, 165, 32, 32);
}

void UfoSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* ufo){
    *(ufo + 0) = esat::SubSprite(atlas, 198, 99, 32, 32);
}

void SpaceShipSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* ship){
    *(ship + 0) = esat::SubSprite(atlas, 231, 132, 32, 32);
}

void JesusSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* jesus){
    *(jesus + 0) = esat::SubSprite(atlas, 231, 99, 32, 32);
}

void FrogSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* frog){
    *(frog + 0) = esat::SubSprite(atlas, 198, 132, 32, 32);
}

void ColorSubSprites(esat::SpriteHandle atlas, EntityInfo* entity){
    entity->color.colors = InitiateSprites();
    *(entity->color.colors + 0) = esat::SubSprite(atlas, 33, 198, 32, 32); // Blue 
    *(entity->color.colors + 1) = esat::SubSprite(atlas, 66, 198, 32, 32); // Red
    *(entity->color.colors + 2) = esat::SubSprite(atlas, 99, 198, 32, 32); // Purple
    *(entity->color.colors + 3) = esat::SubSprite(atlas, 132, 198, 32, 32); // Green
    *(entity->color.colors + 4) = esat::SubSprite(atlas, 165, 198, 32, 32); // Cyan
    *(entity->color.colors + 5) = esat::SubSprite(atlas, 198, 198, 32, 32); // Yellow
    *(entity->color.colors + 6) = esat::SubSprite(atlas, 231, 198, 32, 32); // White
    *(entity->color.colors + 7) = esat::SubSprite(atlas, 99, 214, 32, 16); // Half-purple
}

void PowerSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* power_ups){
    *(power_ups + 0) = esat::SubSprite(atlas, 66, 99, 32, 32); // Amethyst 
    *(power_ups + 1) = esat::SubSprite(atlas, 132, 99, 32, 32); // Diamond
    *(power_ups + 2) = esat::SubSprite(atlas, 132, 132, 32, 32); // Toxic 
    *(power_ups + 3) = esat::SubSprite(atlas, 99, 132, 32, 32); // Tri-Force ? 
    *(power_ups + 4) = esat::SubSprite(atlas, 66, 132, 32, 32); // Topaz
    *(power_ups + 5) = esat::SubSprite(atlas, 99, 99, 32, 32); // Fuel
}

void HeadSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* hed){
    *(hed + 0) = esat::SubSprite(atlas, 0, 0, 32, 32);
    *(hed + 1) = esat::SubSprite(atlas, 33, 0, 32, 32);
}

void BodySubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* bod){
    *(bod + 0) = esat::SubSprite(atlas, 0, 33, 32, 32);
    *(bod + 1) = esat::SubSprite(atlas, 0, 66, 32, 32);
    *(bod + 2) = esat::SubSprite(atlas, 0, 99, 32, 32);
    *(bod + 3) = esat::SubSprite(atlas, 0, 132, 32, 32);
    *(bod + 4) = esat::SubSprite(atlas, 33, 33, 32, 32);
    *(bod + 5) = esat::SubSprite(atlas, 33, 66, 32, 32);
    *(bod + 6) = esat::SubSprite(atlas, 33, 99, 32, 32);
    *(bod + 7) = esat::SubSprite(atlas, 33, 132, 32, 32);
}

void ExplosionSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* ex){
    *(ex + 0) = esat::SubSprite(atlas, 133, 165, 63, 32);
    *(ex + 1) = esat::SubSprite(atlas, 67, 165, 63, 32);
    *(ex + 2) = esat::SubSprite(atlas, 2, 165, 63, 32);
}

void CombustionSubSprites(esat::SpriteHandle atlas, esat::SpriteHandle* comb){
    *(comb + 0) = esat::SubSprite(atlas, 165, 99, 32, 32);
    *(comb + 1) = esat::SubSprite(atlas, 165, 132, 32, 32);
}

void ShipLevel0(esat::SpriteHandle atlas, esat::SpriteHandle* ship){
    *(ship + 0) = esat::SubSprite(atlas, 66, 0, 32, 32);
    *(ship + 1) = esat::SubSprite(atlas, 66, 33, 32, 32);
    *(ship + 2) = esat::SubSprite(atlas, 66, 66, 32, 32);
}

void ShipLevel4(esat::SpriteHandle atlas, esat::SpriteHandle* ship){
    *(ship + 0) = esat::SubSprite(atlas, 99, 0, 32, 32);
    *(ship + 1) = esat::SubSprite(atlas, 99, 33, 32, 32);
    *(ship + 2) = esat::SubSprite(atlas, 99, 66, 32, 32);
}

void ShipLevel8(esat::SpriteHandle atlas, esat::SpriteHandle* ship){
    *(ship + 0) = esat::SubSprite(atlas, 132, 0, 32, 32);
    *(ship + 1) = esat::SubSprite(atlas, 132, 33, 32, 32);
    *(ship + 2) = esat::SubSprite(atlas, 132, 66, 32, 32);
}

void ShipLevel12(esat::SpriteHandle atlas, esat::SpriteHandle* ship){
    *(ship + 0) = esat::SubSprite(atlas, 165, 0, 32, 32);
    *(ship + 1) = esat::SubSprite(atlas, 165, 33, 32, 32);
    *(ship + 2) = esat::SubSprite(atlas, 165, 66, 32, 32);
}

void SwitchUpShips(esat::SpriteHandle atlas, esat::SpriteHandle* ship, LevelHandling* lvl){
    switch(lvl->ship_level){
        case 0:
            ShipLevel0(atlas, ship);
        break;
        case 4:
            ShipLevel4(atlas, ship);
        break;
        case 8:
            ShipLevel8(atlas, ship);
        break;
        case 12:
            ShipLevel12(atlas, ship);
        break;
    }
}

void SwitchUpEnemies(esat::SpriteHandle atlas, esat::SpriteHandle* enemies,
                     EntityInfo* foe, LevelHandling* lvl, unsigned char fps){
    switch(lvl->level){
        case 0:
            MeteoriteSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 25;
            if(g_frames%(fps / 7) == 0){
                ++foe->sprite %= 2;
            }
        break;
        case 1:
            CloudSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 80;
            if(g_frames%(fps / 7) == 0){
                ++foe->sprite %= 2;
            }
        break;
        case 2:
            BubbleSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 40;
            if(g_frames%(fps / 7) == 0){
                ++foe->sprite %= 2;
            }
        break;
        case 3:
            JetSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 55;
        break;
        case 4:
            UfoSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 50;
        break;
        case 5:
            JesusSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 60;
        break;
        case 6:
            SpaceShipSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 25;
        break;
        case 7:
            FrogSubSprites(atlas, enemies);
            esat::DrawSprite(*(enemies + foe->sprite), foe->pos);
            foe->score = 50;
        break;
    }

}

void SpritesInMemory(){
    head = InitiateSprites();
    body = InitiateSprites();
    ships = InitiateSprites();
    power_ups = InitiateSprites();
    enemies = InitiateSprites();
    explosion = InitiateSprites();
    combustion = InitiateSprites();
}

void FreeSprites(){
    free(head); head = nullptr;
    free(body); body = nullptr;
    free(ships); ships = nullptr;
    free(power_ups); power_ups = nullptr;
    free(enemies); enemies = nullptr;
    free(explosion); explosion = nullptr;
    free(combustion); combustion = nullptr;
}

void ReleaseSprites(){
    esat::SpriteRelease(ships);
    esat::SpriteRelease(enemies);
    esat::SpriteRelease(power_ups);
    esat::SpriteRelease(head);
    esat::SpriteRelease(body);
    esat::SpriteRelease(explosion);
    esat::SpriteRelease(combustion);
    esat::SpriteRelease(atlas);
    esat::SpriteRelease(playing_ground);
    esat::SpriteRelease(playing_ground2);
}

void FreeColors(EntityInfo* entity){
    for(int i = 0; i < 8; i++){
        free((entity->color.colors + i)); entity->color.colors = nullptr;
    }
}