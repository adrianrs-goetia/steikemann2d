extends PlayerState
class_name PlayerStateOnGround

var move_horizontal = 0.0
var coyote_time = Timestamp.new()

func enter() -> Type:
    move_horizontal = 0.0
    coyote_time.timestamp()
    return Type.NONE

func exit():
    pass

func input(event: InputEvent) -> Type:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    if event.is_action_pressed(PlayerInput.jump):
        return Type.JUMP
    return Type.NONE

func integrate_forces(state: PhysicsDirectBodyState3D) -> Type:
    DebugDraw3D.draw_position(Transform3D(player.transform), Color.RED, state.step)

    var onground_normals: Array[Vector3] = []

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            onground_normals.append(normal)

    if !onground_normals.size() and coyote_time.timeout(Params.player_coyote_time):
        return Type.INAIR

    var average_normal = Vector3()
    for normal in onground_normals:
        average_normal += normal
    average_normal /= onground_normals.size() as float
    average_normal.normalized()

    var movement_dir = _get_movement_dir(average_normal)

    _set_movement_velocity(movement_dir, state.step)
    _toggle_friction()
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step

    return Type.NONE

func _get_movement_dir(normal: Vector3) -> Vector3:
    var vel_unit: Vector3
    if abs(move_horizontal) > 0.1:
        vel_unit = Vector3(move_horizontal, 0, 0).normalized()
    elif player.linear_velocity.length_squared() > 0.2:
        vel_unit = player.linear_velocity.normalized()
    else:
        return Vector3()
    var right = normal.cross(vel_unit)
    return right.cross(normal)

func _set_movement_velocity(dir: Vector3, delta: float) -> void:
    if dir == Vector3.ZERO:
        return

    var old_speed = abs(player.linear_velocity.x)
    if player.linear_velocity.length_squared() > 0.2:
        old_speed = player.linear_velocity.project(dir).length()
    assert(old_speed != NAN)

    var new_speed = old_speed
    if abs(move_horizontal) > 0.1:
        new_speed = move_toward(old_speed, abs(move_horizontal) * Params.player_move_speed, delta * Params.player_move_acceleration)
    else:
        new_speed = move_toward(old_speed, 0.0, delta * Params.player_move_deceleration)

    player.linear_velocity = dir * new_speed

func _round_to_one(x: float) -> int:
    return -1 if x < 0 else 1;

func _toggle_friction() -> void:
    if abs(move_horizontal) > 0.0:
        player.physics_material_override.friction = 0.0
    else:
        player.physics_material_override.friction = 1.0
