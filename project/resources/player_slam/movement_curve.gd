extends Curve3D
class_name MovementCurve

@export var start_angle: float = -30.0
@export var end_angle: float = 50.0

@export var duration: float = 0.7
@export var timing_curve: Curve
var curve_time: CurveTime

func init(start: Vector3, end: Vector3) -> void:
    assert(timing_curve)
    curve_time = CurveTime.new(duration)
    clear_points()
    add_point(start)
    add_point(end)

    set_point_out(0, _get_point_dir(start, end, start_angle))
    set_point_in(1, _get_point_dir(end, start, end_angle))

func _get_point_dir(dir_start: Vector3, dir_end: Vector3, angle: float) -> Vector3:
    var dir = Vector3(dir_end - dir_start).normalized()
    var ortho = dir.cross(Vector3.UP)

    var _angle = deg_to_rad(angle)
    var quat = Quaternion.IDENTITY
    var a = sin(_angle / 2)
    quat.x = a * ortho.x
    quat.y = a * ortho.y
    quat.z = a * ortho.z
    quat.w = cos(_angle / 2)
    quat = quat.normalized()

    return quat * dir

func process(delta: float) -> Vector3:
    var sample_point = timing_curve.sample(curve_time.step(delta))
    return samplef(sample_point)

func done() -> bool:
    return curve_time.done()