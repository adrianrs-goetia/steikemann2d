extends PlayerState
class_name PlayerStateInAir

var move_horizontal = 0.0
var min_process_time = Timestamp.new()

func enter() -> Type:
    player.physics_material_override.friction = 0.0
    min_process_time.timestamp()
    return Type.NONE

func exit():
    player.physics_material_override.friction = 1.0

func input(_event: InputEvent) -> Type:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    return Type.NONE

func integrate_forces(state: PhysicsDirectBodyState3D) -> Type:
    super.integrate_forces(state)
    player.linear_velocity.x = move_horizontal * state.step * Params.player_inair_move_speed

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            if min_process_time.timeout(30):
                return Type.ONGROUND

    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step
    
    return Type.NONE
