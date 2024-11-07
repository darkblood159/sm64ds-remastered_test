#include "src/game/envfx_snow.h"

// 0x0E000608
const GeoLayout thi_geo_000608[] = {
    GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
    GEO_OPEN_NODE(),
        GEO_ZBUFFER(1),
        GEO_OPEN_NODE(),
            GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 20000, geo_camera_fov),
            GEO_OPEN_NODE(),
                GEO_CAMERA(1, 0, 2000, 6000, 0, 0, 0, geo_camera_main),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_OPAQUE, thi_seg7_dl_07005260),
                    GEO_DISPLAY_LIST(LAYER_OPAQUE, thi_seg7_dl_07006968),
                    GEO_DISPLAY_LIST(LAYER_ALPHA, thi_seg7_dl_07007008),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, thi_seg7_dl_070072E8),
                #if !PROPER_TREE_SHADOWS
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, thi_seg7_dl_070073C0),
                #endif
                    GEO_ASM(   0, geo_movtex_pause_control),
                    GEO_ASM(0x1301, geo_movtex_draw_water_regions),
                    GEO_RENDER_OBJ(),
                    GEO_ASM(   0, geo_envfx_main),
                    GEO_ASM(0, sky_3d),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_ASM(0, geo_cannon_circle_base),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
