extends PlayerState
class_name PlayerStateInAir

var move_horizontal = 0.0
var min_process_time = Timestamp.new()

func get_name() -> String:
    return "InAir"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    player.physics_material_override.friction = 0.0
    min_process_time.timestamp()
    return null

func exit() -> void:
    player.physics_material_override.friction = 1.0

func input(_event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            if min_process_time.timeout(60):
                return PlayerStateOnGround.new(move_horizontal)

    _set_movement_velocity(state.step)
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step
    
    return null

func _set_movement_velocity(delta: float) -> void:
    var old_speed = player.linear_velocity.x
    var new_speed = old_speed

    if abs(move_horizontal) > 0.1:
        new_speed = move_toward(old_speed, move_horizontal * Params.player_move_speed, delta * Params.player_move_acceleration)
    else:
        new_speed = move_toward(old_speed, 0.0, delta * Params.player_move_deceleration)

    player.linear_velocity.x = new_speed