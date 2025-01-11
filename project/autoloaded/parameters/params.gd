extends Node


### Player
var gravity: float = 9.81
var player_floor_angle: float = 0.7825 # 38.5 degrees
var player_gravity_scale: float = 2.8

var player_model_rotation_threshold: float = 0.2
var player_model_rotation_angle: float = 70.0:
    get:
        # print("model rotation: " + str(player_model_rotation_angle))
        return player_model_rotation_angle

# onground
var player_move_speed: float = 8.4
var player_move_acceleration: float = 70.0
var player_move_deceleration: float = 50.0
var player_coyote_time = 300 # in milliseconds

## anim
var player_idle_run_blend_speed = 50.0

# jump
var player_jump_time = 100
var player_jump_strength: float = 7.0

# inair
var player_inair_min_process_time = 30 # in milliseconds
var player_ground_check_raycast_length = 0.2

# slope
# var player_slope_angle: float = 0.2079 # 78 degrees
var player_slope_angle: float = 0.3255 # 71 degrees
var player_slope_slide_speed: float = 17.0
var player_slope_slide_acceleration: float = 200.0

# attack
var player_attack_time = 700


### AubergineDog
var audog_gravity_scale = 2.0

## bk 
# sticky
var audog_bk_sticky_collision_inactive_time = 300

# floaty
var audog_bk_floaty_force = 100.0

# bouncy
var audog_bk_bouncy_strength = 1.0
var audog_bk_bouncy_max_velocity = 10.0
var audog_bk_bouncy_collision_interval = 100
