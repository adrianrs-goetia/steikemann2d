@tool
extends SpringArm3D
class_name PlayerCamera

var player: PlayerNode = null

func _enter_tree() -> void:
	player = get_parent() as PlayerNode
	add_excluded_object(get_parent())

func _process(delta: float) -> void:
	var camera_basis = global_transform.basis
	camera_basis.z *= -1
	camera_basis.y *= -1
	var camera = $Camera3D
	camera.global_transform = Transform3D(camera_basis, global_position + (camera_basis.z * spring_length))
