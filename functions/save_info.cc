#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

void LoadInformation(){
    if((first_player = fopen("figures.dat", "rb")) == NULL){
        printf("Error. No Previous Info Saved\n");
    } else {
        while(fread(&samia, sizeof(samia), 1, first_player)){
            player = (Entity*)realloc(player, (kNPlayerAmount) * sizeof(EntityInfo));
            *(player + kNPlayerAmount) = samia;
        }
        fclose(first_player);
    }
}