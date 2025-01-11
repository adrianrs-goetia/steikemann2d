extends BoneAttachment3D
class_name BlomkaolNode

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
var current_power = Power.NONE
const other_scale = 0.1 # hack method to deal with player model scaling...


func apply_power_to_owner(power: Power):
	if _attachment == Attachment.ON_PLAYER:
		return
	if get_parent() is AudogNode:
		get_parent().process_blomkaol_power(power)

func attach_to_other(node: Node3D, offset: Vector3):
	var p = get_parent()
	if p == node:
		return
	if p is AudogNode:
		p.detach_blomkaol()

	_attachment = Attachment.ON_OTHER
	reparent(node)
	set_physics_process(true)
	global_position = node.global_position + offset
	set_use_external_skeleton(false)
	scale = Vector3(other_scale, other_scale, other_scale)
	apply_power_to_owner(current_power)

	if node is AudogNode:
		node.attach_blomkaol(self)

func attach_to_player(player: PlayerNode):
	if get_parent() is AudogNode:
		get_parent().detach_blomkaol()

	apply_power_to_owner(Power.NONE) # reset owner

	_attachment = Attachment.ON_PLAYER
	set_physics_process(false)
	reparent(player)
	set_use_external_skeleton(true)
	set_external_skeleton("../Model/Potitt_rig/root_jnt/Skeleton3D")

func set_blomkaol_power(power: Power, gui: PlayerGui):
	# toggle power when same button is pressed
	if power == current_power and power != Power.NONE:
		current_power = Power.NONE
	else:
		current_power = power

	var color: Color
	match current_power:
		Power.NONE: color = Color.WHITE
		Power.STICKY: color = Color.GREEN
		Power.BOUNCY: color = Color.ORANGE
		Power.ROCKY: color = Color.DIM_GRAY
		Power.FLOATY: color = Color.PURPLE
	$MeshInstance3D.get_active_material(0).set("shader_parameter/color", color)
	gui.set_blomkaol_power(current_power)

	apply_power_to_owner(current_power)
