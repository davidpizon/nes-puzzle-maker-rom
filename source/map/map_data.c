#include "source/map/map_data.h"
#include "source/game_data/game_data.h"
#include "source/library/bank_helpers.h"

CODE_BANK(PRG_BANK_MAP_DATA);

const unsigned char builtInMapData[256] = {
    // 0x00:Id
    GAME_DATA_VERSION_ID,
    // 0x01:Tileset id
    0x00,
    // 0x02:Game Style
    GAME_STYLE_MAZE,
    // 0x03:Song Id
    0x00,
    // 0x04:Sfx Id
    0x00,
    // 0x05:Gfx Id
    0x00,
    // 0x06:Unused/reserved data (0x0a bytes)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // 0x10:Game Title (12 bytes, 6bit encoded, 16 bytes unencoded)
    // FIXME: Fake name
    0x11, 0x25, 0x73, 0x17, 0x02, 0x00, 0x77, 0x00, 0x45, 0x00, 0x00, 0x00, 
    // 0x1c:Author name (12 bytes, same as above)
    0x11, 0x25, 0x73, 0x17, 0x02, 0x00, 0x77, 0x00, 0x45, 0x00, 0x00, 0x00, 
    //0x28:Credits Screen (24 bytes, same as above -- expands to 32)
    0x11, 0x25, 0x73, 0x17, 0x02, 0x00, 0x77, 0x00, 0x45, 0x00, 0x00, 0x00, 
    0x11, 0x25, 0x73, 0x17, 0x02, 0x00, 0x77, 0x00, 0x45, 0x00, 0x00, 0x00, 

    //0x40:Game Data (192 bytes, 24 bytes per level, (8 levels) 6bit encoded; expands to 32 per level, or... theoretically 256)
    
    //L1
    0x01, 0x13, 0x25,
    0xc1, 0x53, 0xe5,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,

    //L2
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0x1c, 0xa8, 0x1c,
    0xc1, 0x53, 0xe5,
    0x01, 0x13, 0x25,

    //L3
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    //L4
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    
    //L5
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    
    //L6
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    
    //L7
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    
    //L8
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00

};




