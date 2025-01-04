extends PlayerState
class_name PlayerStateOnGround

var move_horizontal = 0.0
var coyote_time = Timestamp.new()

func _init(move_x: float) -> void:
    move_horizontal = move_x
    pass

func get_name() -> String:
    return "OnGround"

func enter() -> PlayerState:
    coyote_time.timestamp()
    return null

func exit() -> void:
    pass

func input(event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    if event.is_action_pressed(PlayerInput.jump):
        return PlayerStateJump.new(move_horizontal)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    DebugDraw3D.draw_position(Transform3D(player.transform), Color.RED, state.step)

    var onground_normals: Array[Vector3] = []
    var other_contact_normals: Array[Vector3] = []

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            onground_normals.append(normal)
        else:
            other_contact_normals.append(normal)


    var average_normal = Vector3()
    if onground_normals.size(): # on ground
        coyote_time.timestamp()
        average_normal = _average_unit_vector(onground_normals)
    else:
        # on slope
        if other_contact_normals.size():
            # if _average_unit_vector(other_contact_normals).dot(Globals.up) < Params.player_floor_angle:
            match _get_normal_type(_average_unit_vector(other_contact_normals)):
                NormalType.SLOPE:
                    return PlayerStateInAir.new(move_horizontal)

        # Avoid flaky movement across /\ shaped terrain, use coyote_time
        # to allow player to land again before they move to air state
        # NOTE: should probably do raycast below to check if there is terrain below
        #       actual coyote time might actually require a separate timer
        if coyote_time.timeout(Params.player_coyote_time):
            return PlayerStateInAir.new(move_horizontal)

    var movement_dir = _get_movement_dir(average_normal)

    _set_movement_velocity(movement_dir, state.step)
    _toggle_friction()
    
    # add *2 to gravity to forcibly stick player to ground across /\ terrain
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step * 2
    
    _rotate_model(move_horizontal)

    return null

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

func _toggle_friction() -> void:
    if abs(move_horizontal) > 0.0:
        player.physics_material_override.friction = 0.0
    else:
        player.physics_material_override.friction = 1.0
