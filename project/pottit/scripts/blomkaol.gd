extends BoneAttachment3D
class_name Blomkaol

enum Power {
	NONE,
	STICKY,
	BOUNCY,
	ROCKY,
	FLOATY,
}

enum Attachment {
	ON_PLAYER,
	ON_OTHER,
}

var _attachment = Attachment.ON_PLAYER
var _current_power = Power.NONE
const other_scale = 0.1 # hack method to deal with player model scaling...

func _physics_process(delta: float) -> void:
	apply_power_to_owner(delta, _current_power)

func apply_power_to_owner(delta: float, power: Power):
	if _attachment == Attachment.ON_PLAYER:
		return
	var node = get_parent() as SmackableTest
	if node:
		node.process_blomkaol_power(delta, power)

func attach_to_other(node: Node3D, offset: Vector3):
	_attachment = Attachment.ON_OTHER
	reparent(node)
	set_physics_process(true)
	global_position = node.global_position + offset
	set_use_external_skeleton(false)
	scale = Vector3(other_scale, other_scale, other_scale)

func attach_to_player(player: PlayerNode):
	apply_power_to_owner(get_physics_process_delta_time(), Power.NONE) # reset owner

	_attachment = Attachment.ON_PLAYER
	set_physics_process(false)
	reparent(player)
	set_use_external_skeleton(true)
	set_external_skeleton("../Model/Potitt_rig/root_jnt/Skeleton3D")

func set_blomkaol_power(power: Power, gui: PlayerGui):
	var color: Color
	# toggle power when same button is pressed
	if power == _current_power and power != Power.NONE:
		_current_power = Power.NONE
	else:
		_current_power = power
	match _current_power:
		Power.NONE: color = Color.WHITE
		Power.STICKY: color = Color.GREEN
		Power.BOUNCY: color = Color.ORANGE
		Power.ROCKY: color = Color.DIM_GRAY
		Power.FLOATY: color = Color.PURPLE
	$MeshInstance3D.get_active_material(0).set("shader_parameter/color", color)
	gui.set_blomkaol_power(_current_power)
