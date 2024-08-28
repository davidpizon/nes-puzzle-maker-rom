#include "source/c/menus/credits.h"
#include "source/c/mapper.h"
#include "source/c/library/user_data.h"
#include "source/c/configuration/system_constants.h"
#include "source/c/globals.h"
#include "source/c/neslib.h"
#include "source/c/configuration/game_states.h"
#include "source/c/menus/text_helpers.h"
#include "source/c/map/map.h"
#include "source/c/graphics/fade_animation.h"
#include "source/c/menus/input_helpers.h"
#include "source/c/library/user_data.h"

#pragma code-name ("MENUS")
#pragma rodata-name ("MENUS")

void draw_win_screen() {
    ppu_off();
    clear_screen_with_border();
    scroll(0, 0);

    if (disableStatsScreen) {
        ppu_on_all();
        return;
    }


    // Add whatever you want here; NTADR_A just picks a position on the screen for you. Your options are 0, 0 to 32, 30
    put_str(NTADR_A(8, 5), "Congratulations!");

    put_str(NTADR_A(9, 12), "You have won! ");

    put_str(NTADR_A(5, 24), "Your time:       ");

    // 60 frames/second, so get down to seconds
    tempInt1 = (frameCount - gameTime) / 60;
    // Build it up in reverse...
    screenBuffer[4] = (tempInt1 % 60) % 10;
    screenBuffer[3] = (tempInt1 % 60) / 10;
    // Down to minutes
    tempInt1 /= 60;
    screenBuffer[2] = (tempInt1 % 100) % 10;
    screenBuffer[1] = (tempInt1 % 100) / 10;

    vram_put('0' + screenBuffer[1] + 0x60);
    vram_put('0' + screenBuffer[2] + 0x60);
    vram_put(':' + 0x60);
    vram_put('0' + screenBuffer[3] + 0x60);
    vram_put('0' + screenBuffer[4] + 0x60);

    tempChar1 = 0;
    switch (currentGameStyle) {
        case GAME_STYLE_MAZE:
            // Do nothing; nothing special to show
            tempChar1 = 1;
            break;
        case GAME_STYLE_COIN:
            load_coinsCollectedText_to_buffer(BANK_MENUS);
            put_str(NTADR_A(5, 22), userDataBuffer);
            tempInt1 = gameCollectableCount;
            break;
        case GAME_STYLE_CRATES:
            load_cratesRemovedText_to_buffer(BANK_MENUS);
            put_str(NTADR_A(5, 22), userDataBuffer);
            tempInt1 = gameCrates;
            break;
    }
    // NOTE: If there are space issues, we might benefit from incrementing these 
    // "smartly" and replacing this with & 0x0f, &0xf0, (>>8)& 0x0f, etc
    if (tempChar1 == 0) {
        vram_put('0' + ((tempInt1 / 100) % 10) + 0x60);
        vram_put('0' +((tempInt1 / 10) % 10) + 0x60);
        vram_put('0' + ((tempInt1 % 10)) + 0x60);
    }

    // Hide all existing sprites
    oam_clear();
    ppu_on_all();

    fade_in();
    wait_for_start();
    fade_out();

}

void load_credits_user_data();
void draw_credits_screen() {
    ppu_off();
    scroll(0, 0);


    vram_adr(0x2000);
    load_credits_user_data();

    // Hide all existing sprites
    oam_clear();
    ppu_on_all();

    fade_in();
    wait_for_start();
    fade_out();
}

#pragma code-name ("CODE")
#pragma rodata-name ("CODE")

void load_credits_user_data() {
    unrom_set_prg_bank(BANK_USER_DATA);
    vram_write((unsigned char*)&(user_creditsScreenData[0]), 0x400);
    unrom_set_prg_bank(BANK_MENUS);
}

