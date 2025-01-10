extends BoneAttachment3D
class_name Blomkaol

enum {
	NONE,
	STICKY,
	BOUNCY,
	ROCKY,
	FLOATY,
}

var current_power: int = NONE

func set_blomkaol_power(power: int, gui: PlayerGui):
	var color: Color
	# toggle power when same button is pressed
	if power == current_power and power != NONE:
		current_power = NONE
	else:
		current_power = power
	match current_power:
		NONE: color = Color.WHITE
		STICKY: color = Color.GREEN
		BOUNCY: color = Color.ORANGE
		ROCKY: color = Color.DIM_GRAY
		FLOATY: color = Color.PURPLE
	$MeshInstance3D.get_active_material(0).set("shader_parameter/color", color)
	gui.set_blomkaol_power(current_power)
