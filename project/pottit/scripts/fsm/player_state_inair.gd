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
    var slope_wall_collision: Array[Vector3] = []

    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        match _get_collision_normal(normal):
            CollisionNormal.GROUND:
                if min_process_time.timeout(Params.player_inair_min_process_time):
                    return PlayerStateOnGround.new(move_horizontal)
            CollisionNormal.SLOPE:
               slope_wall_collision.append(normal)


    # todo: slope state
    var average_slope_wall_collision = Vector3()
    if slope_wall_collision.size() > 0:
        average_slope_wall_collision = _average_unit_vector(slope_wall_collision)
        _slope_slide(average_slope_wall_collision, state.step)
        _rotate_model(player.linear_velocity.x)
    else:
        _set_movement_velocity(state.step)
        _rotate_model(move_horizontal)
    
    return null

func _set_movement_velocity(delta: float) -> void:
    player.linear_velocity.x = _move_toward(move_horizontal, delta)
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * delta

func _slope_slide(slope_wall_dir: Vector3, delta: float) -> void:
    var right = slope_wall_dir.cross(-Globals.up)
    var slope = right.cross(slope_wall_dir)
    
    var old_speed = player.linear_velocity.length()
    var new_speed = move_toward(old_speed, Params.player_slope_slide_speed, delta * Params.player_slope_slide_acceleration)
    player.linear_velocity = slope * new_speed

    return