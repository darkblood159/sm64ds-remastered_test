#ifndef MARIO_STEP_H
#define MARIO_STEP_H

#include <PR/ultratypes.h>

#include "types.h"

struct BullyCollisionData {
  /*0x00*/ f32 conversionRatio;
  /*0x04*/ f32 radius;
  /*0x08*/ f32 posX;
  /*0x0C*/ f32 posZ;
  /*0x10*/ f32 velX;
  /*0x14*/ f32 velZ;
};

extern struct Surface gWaterSurfacePseudoFloor;

f32 get_additive_y_vel_for_jumps(void);
void stub_player_step_1(struct PlayerState *);
void stub_player_step_2(void);

void mario_bonk_reflection(struct PlayerState *, u32);
void transfer_bully_speed(struct BullyCollisionData *obj1, struct BullyCollisionData *obj2);
BAD_RETURN(s32) init_bully_collision_data(struct BullyCollisionData *data, f32 posX, f32 posZ,
                                          f32 forwardVel, s16 yaw, f32 conversionRatio, f32 radius);
u32 mario_update_quicksand(struct PlayerState *, f32);
u32 mario_push_off_steep_floor(struct PlayerState *, u32, u32);
u32 mario_update_moving_sand(struct PlayerState *);
u32 mario_update_windy_ground(struct PlayerState *);
void stop_and_set_height_to_floor(struct PlayerState *);
s32 stationary_ground_step(struct PlayerState *);
s32 perform_ground_step(struct PlayerState *);
s32 perform_air_step(struct PlayerState *, u32);

#endif // MARIO_STEP_H
