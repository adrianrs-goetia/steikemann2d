@tool
extends Node3D

@export var curve: Curve
@export var curve3d: Curve3D

@export var start_angle: float = 30.0
@export var end_angle: float = 30.0

var child: Node3D

var curve_time: CurveTime

func _enter_tree() -> void:
    assert(curve)
    var timing = curve.get_meta("time") as float
    curve_time = CurveTime.new(timing)
    child = $EndPos
    curve3d = Curve3D.new()

    call_deferred("setup_curve3d")
    curve3d.set_point_out(0, _get_point_dir(Vector3.ZERO, child.position, start_angle))
    curve3d.set_point_in(1, _get_point_dir(child.position, Vector3.ZERO, start_angle))

func setup_curve3d():
    assert(curve3d != null)
    var start = Vector3.ZERO
    var end = child.position
    curve3d.add_point(start)
    curve3d.add_point(end)

func _get_point_dir(dir_start: Vector3, dir_end: Vector3, angle: float) -> Vector3:
    var dir = Vector3(dir_end - dir_start).normalized()
    var ortho = dir.cross(Vector3.UP)

    var _angle = deg_to_rad(angle)
    var quat = Quaternion.IDENTITY
    quat.x = sin(_angle / 2) * ortho.x
    quat.y = sin(_angle / 2) * ortho.y
    quat.z = sin(_angle / 2) * ortho.z
    quat.w = cos(_angle / 2)
    quat = quat.normalized()

    return quat * dir

func _process(delta: float) -> void:
    assert(curve != null)
    assert(curve3d != null)
    assert(child != null)
    
    var sample_point = curve.sample(curve_time.step(delta))
    var pos = curve3d.samplef(sample_point)
    DebugDraw3D.draw_position(Transform3D(Basis(), pos), Color.DARK_MAGENTA, delta)
    pass
