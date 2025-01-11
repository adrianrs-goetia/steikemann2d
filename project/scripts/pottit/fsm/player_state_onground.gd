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
    player.model.on_ground()
    return null

func exit() -> void:
    pass

func input(event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    if event.is_action_pressed(PlayerInput.jump):
        return PlayerStateJump.new(move_horizontal)
    
    if event.is_action_pressed(PlayerInput.attack):
        return PlayerStateAttack.new(move_horizontal)
    
    if event.is_action_pressed(PlayerInput.pickup_throw):
        _pickup_throw()
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    # DebugDraw3D.draw_position(Transform3D(player.transform), Color.RED, state.step / 2.0)

    var onground_normals: Array[Vector3] = []
    var other_contact_normals: Array[Vector3] = []
    var gravity_multiplier: float = 2.0

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
        var ground_below = _ground_directly_below(state.center_of_mass, Params.player_ground_check_raycast_length)

        # on slope
        if other_contact_normals.size() and !_contains_collision_normal(CollisionNormal.WALL, other_contact_normals) and ground_below:
            match _get_collision_normal(_average_unit_vector(other_contact_normals)):
                CollisionNormal.SLOPE:
                    return PlayerStateInAir.new(move_horizontal)
        elif !ground_below:
            # Avoid flaky movement around /\
            if coyote_time.timeout(Params.player_coyote_time):
                return PlayerStateInAir.new(move_horizontal)
        elif !_ground_directly_below(state.center_of_mass, Params.player_ground_check_raycast_length * 2):
            return PlayerStateInAir.new(move_horizontal)

    var movement_dir = _get_movement_dir(average_normal)

    var speed = _set_movement_velocity(movement_dir, state.step)
    _toggle_friction()
    
    # add *2 to gravity to forcibly stick player to ground across /\ terrain
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * state.step * gravity_multiplier
    
    ##
    # audio/visual stuff
    _rotate_model(move_horizontal)
    player.model.set_idle_run_blend(speed)

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

func _set_movement_velocity(dir: Vector3, delta: float) -> float:
    if dir == Vector3.ZERO:
        return 0.0

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
    return new_speed

func _toggle_friction() -> void:
    if abs(move_horizontal) > 0.0:
        player.physics_material_override.friction = 0.0
    else:
        player.physics_material_override.friction = 1.0

func _pickup_throw():
    if player.pickup_socket.get_child_count(): # throw other
        var other = player.pickup_socket.get_child(0) as AubergineDogNode
        assert(other != null)

        other.on_throw(Vector3(_get_forward_x() * 6, 5, 0))

    else: # pickup other
        player.pickup.enabled = true
        
        for i in player.pickup.get_collision_count():
            var other = player.pickup.get_collider(i) as AubergineDogNode
            if other:
                other.on_pickup(player.pickup_socket)
                break
