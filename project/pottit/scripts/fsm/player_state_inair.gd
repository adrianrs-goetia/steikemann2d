extends PlayerState
class_name PlayerStateInAir

var move_horizontal = 0.0
var min_process_time = Timestamp.new()

func get_name() -> String:
    return "InAir"

func _init() -> void:
    pass

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
    super.integrate_forces(state)
    player.linear_velocity.x = move_horizontal * state.step * Params.player_inair_move_speed

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            if min_process_time.timeout(30):
                return PlayerStateOnGround.new()

    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step
    
    return null
