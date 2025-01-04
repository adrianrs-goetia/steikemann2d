extends PlayerState
class_name PlayerStateJump

var move_horizontal = 0.0

func get_name() -> String:
    return "Jump"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    player.linear_velocity.y = Params.player_jump_strength
    player.physics_material_override.friction = 0.0
    return PlayerStateInAir.new(move_horizontal)

func exit() -> void:
    pass

# func input(_event: InputEvent) -> PlayerState:
#     move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
#     return Type.NONE

# func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
#     player.linear_velocity.x = move_horizontal * state.step * Params.player_inair_move_speed

#     min_process_frame = max(min_process_frame - 1, 0)

#     for i in state.get_contact_count():
#         var normal = state.get_contact_local_normal(i)
#         if normal.dot(Globals.up) > Params.player_floor_angle && min_process_frame == 0:
#             return Type.ONGROUND

#     player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step
    
#     player.position.z = 0 # sanity adjustment

#     return Type.NONE
