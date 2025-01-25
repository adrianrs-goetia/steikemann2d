@tool
extends Node3D

var pos = Vector3(0, 0, 0)
var length = 1.0

var X = Vector3(1, 0, 0)
var Y = Vector3(0, 1, 0)
var Z = Vector3(0, 0, 1)
var dir = Vector3(X).normalized()

var SIZE = Vector3(1, 1, 1)

var angle_inner: float = 0.0
var angle: float = 0.0

func _process(delta: float) -> void:
	# quat_yaw_pitch(delta)
	quat_orthogonal(delta)
	
func quat_orthogonal(delta: float) -> void:
	angle_inner += delta * 0.55
	angle = angle_inner

	var quat = Quaternion.IDENTITY
	quat.x = sin(angle / 2)
	quat.y = sin(angle / 2)
	quat.z = sin(angle / 2)
	quat.w = cos(angle / 2)
	quat = quat.normalized()
	DebugDraw3D.draw_box(pos, quat, SIZE, Color.WHITE, delta)

	var forward = quat * Vector3(X + Y + Z).normalized()
	var ortho = forward.cross(Y).normalized()
	var ortho2 = ortho.cross(forward).normalized()
	DebugDraw3D.draw_line(pos, pos + (forward), Color.RED, delta)
	DebugDraw3D.draw_line(pos, pos + (quat * ortho), Color.GREEN, delta)
	DebugDraw3D.draw_line(pos, pos + (quat * ortho2), Color.BLUE, delta)

func quat_yaw_pitch(delta: float) -> void:
	angle_inner += delta * 0.6
	angle = angle_inner

	var quat_x = Quaternion.IDENTITY
	quat_x = quat_x.normalized()
	DebugDraw3D.draw_line(pos, pos + (quat_x * X), Color.RED, delta)

	var quat_z = Quaternion.IDENTITY
	quat_z.z = sin(cos(angle_inner * 1.2) / 2)
	quat_z.w = cos(cos(angle_inner * 1.2) / 2)
	quat_z = quat_z.normalized()
	DebugDraw3D.draw_line(pos, pos + (quat_z * X), Color.BLUE, delta)

	var quat_y = Quaternion.IDENTITY
	quat_y.y = sin(angle / 2)
	quat_y.w = cos(angle / 2)
	quat_y = quat_y.normalized()
	DebugDraw3D.draw_line(pos, pos + (quat_y * X), Color.YELLOW, delta)

	var q = quat_y * quat_z
	DebugDraw3D.draw_line(pos, pos + (q * X), Color.WHITE, delta)
