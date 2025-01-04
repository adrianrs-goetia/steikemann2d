extends PlayerState
class_name PlayerStateOnGround

var move_horizontal = 0.0
var variance_avoidance = Timestamp.new()

func enter() -> Type:
    move_horizontal = 0.0
    variance_avoidance.timestamp()
    return Type.NONE

func exit():
    pass

func input(event: InputEvent) -> Type:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    if event.is_action_pressed(PlayerInput.jump):
        return Type.JUMP
    return Type.NONE

func integrate_forces(state: PhysicsDirectBodyState3D) -> Type:
    var onground = false
    DebugDraw3D.draw_position(Transform3D(player.transform), Color.RED, state.step)

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            onground = true

    if !onground and variance_avoidance.timeout(Params.player_coyote_time):
        return Type.INAIR

    _set_movement_velocity(state.step)
    _toggle_friction()
    # player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step


    return Type.NONE

func _set_movement_velocity(delta: float) -> void:
    var oldvel = player.linear_velocity.x
    var newvel = oldvel
    if abs(move_horizontal) > 0.2:
        newvel = move_toward(oldvel, move_horizontal * Params.player_move_speed, delta  * Params.player_move_acceleration)
    else:
        newvel = move_toward(oldvel, 0.0, delta  * Params.player_move_deceleration)
    player.linear_velocity.x = newvel

func _toggle_friction() -> void:
    if abs(move_horizontal) > 0.0:
        player.physics_material_override.friction = 0.0
    else:
        player.physics_material_override.friction = 1.0
