extends Node


### Player
const gravity: float = 9.81
const player_floor_angle: float = 0.7825 # 38.5 degrees
const player_gravity_scale: float = 2.8

const player_model_rotation_threshold: float = 0.2
var player_model_rotation_angle: float = 70.0:
    get:
        # print("model rotation: " + str(player_model_rotation_angle))
        return player_model_rotation_angle

# onground
const player_move_speed: float = 8.4
const player_move_acceleration: float = 70.0
const player_move_deceleration: float = 50.0
const player_coyote_time = 300 # in milliseconds

## anim
const player_idle_run_blend_speed = 50.0

# jump
const player_jump_time = 100
const player_jump_strength: float = 7.0

# inair
const player_inair_min_process_time = 30 # in milliseconds
const player_ground_check_raycast_length = 0.2

# slope
# const player_slope_angle: float = 0.2079 # 78 degrees
const player_slope_angle: float = 0.3255 # 71 degrees
const player_slope_slide_speed: float = 17.0
const player_slope_slide_acceleration: float = 200.0

# attack
const player_attack_time = 700


###
