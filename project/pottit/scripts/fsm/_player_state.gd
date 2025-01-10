extends Object
class_name PlayerState # base abstract class

var player: PlayerNode

func get_name() -> String:
    assert(false)
    return ""

func can_enter() -> bool:
    return true

func enter() -> PlayerState:
    return null

func exit() -> void:
    pass

func input(_event: InputEvent) -> PlayerState:
    return null

func integrate_forces(_state: PhysicsDirectBodyState3D) -> PlayerState:
    return null

###########################
### Helper functions
###########################

# Rotate model if move_x is greater then threshold. Otherwise maintain rotation
func _rotate_model(move_x: float) -> void:
    if Params.player_model_rotation_threshold < abs(move_x):
        var dir = _round_to_one(move_x)
        player.model.rotation_degrees.y = Params.player_model_rotation_angle * dir
        player.model.scale.x = -dir

func _round_to_one(x: float) -> int:
    return -1 if x < 0 else 1;

func _x_direction(v1: Vector3, v2: Vector3) -> int:
    var dir = v2 - v1
    return _round_to_one(dir.x)

func _average_unit_vector(arr: Array[Vector3]) -> Vector3:
    var average = Vector3()
    for vec in arr:
        average += vec
    average /= arr.size() as float
    return average.normalized()

### 
func _move_toward(move_x: float, delta: float) -> float:
    var speed = player.linear_velocity.x
    if abs(move_x) > 0.1:
        speed = move_toward(speed, move_x * Params.player_move_speed, delta * Params.player_move_acceleration)
    else:
        speed = move_toward(speed, 0.0, delta * Params.player_move_deceleration)
    return speed

func _ground_directly_below(center: Vector3, length_below: float) -> bool:
    var pos = player.position + center
    var ray_end = player.position - Vector3(0, length_below, 0)
    var space = player.get_world_3d().direct_space_state
    var query = PhysicsRayQueryParameters3D.create(pos, ray_end)
    
    query.exclude = [player.get_rid()]
    var result = space.intersect_ray(query)
    return result.size() as bool

### Evaluating collision normal types
enum CollisionNormal {
    GROUND,
    SLOPE,
    WALL,
}
func _get_collision_normal(normal: Vector3) -> CollisionNormal:
    var dot = normal.dot(Globals.up)
    if Params.player_floor_angle < dot:
        return CollisionNormal.GROUND
    elif Params.player_slope_angle < dot:
        return CollisionNormal.SLOPE
    return CollisionNormal.WALL

func _contains_collision_normal(type: CollisionNormal, normals: Array[Vector3]) -> bool:
    for normal in normals:
        if _get_collision_normal(normal) == type:
            return true
    return false
