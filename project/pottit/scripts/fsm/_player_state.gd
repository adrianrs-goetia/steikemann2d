extends Object
class_name PlayerState

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
    

func _round_to_one(x: float) -> int:
    return -1 if x < 0 else 1;

func _average_unit_vector(arr: Array[Vector3]) -> Vector3:
    var average = Vector3()
    for vec in arr:
        average += vec
    average /= arr.size() as float
    return average.normalized()

enum NormalType {
    GROUND,
    SLOPE,
    WALL,
}
func _get_normal_type(normal: Vector3) -> NormalType:
    var dot = normal.dot(Globals.up)
    if Params.player_floor_angle < dot:
        return NormalType.GROUND
    elif Params.player_slope_angle < dot:
        return NormalType.SLOPE
    return NormalType.WALL