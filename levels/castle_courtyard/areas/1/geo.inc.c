#ifndef TARGET_N64
#include "src/game/envfx_skybox.h"
#endif

const GeoLayout castle_courtyard_area_1[] = {
    GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
    GEO_OPEN_NODE(),
#ifdef TARGET_N64
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_NODE_ORTHO(100),
            GEO_OPEN_NODE(),
                GEO_BACKGROUND(BACKGROUND_OCEAN_SKY, geo_skybox_main),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
#endif
        GEO_ZBUFFER(1),
        GEO_OPEN_NODE(),
            GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 12800, geo_camera_fov),
            GEO_OPEN_NODE(),
                GEO_CAMERA(16, 0, 2000, 6000, 0, 0, 0, geo_camera_main),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_OPAQUE, courtyard_mesh_Visual_Model_mesh_layer_1),
		            GEO_DISPLAY_LIST(LAYER_ALPHA, courtyard_mesh_Visual_Model_mesh_layer_4),
		            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, courtyard_mesh_Visual_Model_mesh_layer_5),
                    GEO_RENDER_OBJ(),
                    GEO_ASM(0, geo_envfx_main),
#ifndef TARGET_N64
                    GEO_ASM(0, sky_3d),
#endif
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
