extends Control
class_name PlayerGui

var blomkaol_selection_visual: Control
var current_blomkaol_power: Blomkaol.Power
var blomkaol_1: Control
var blomkaol_2: Control
var blomkaol_3: Control
var blomkaol_4: Control

func _enter_tree() -> void:
	blomkaol_selection_visual = $BlomkaolSelection
	blomkaol_1 = $"BlomkaolPowers/1"
	blomkaol_2 = $"BlomkaolPowers/2"
	blomkaol_3 = $"BlomkaolPowers/3"
	blomkaol_4 = $"BlomkaolPowers/4"

func set_blomkaol_power(power: int):
	if power == Blomkaol.Power.NONE:
		blomkaol_selection_visual.visible = false
		return
	
	var selection: Control
	match power:
		Blomkaol.Power.STICKY: selection = blomkaol_1
		Blomkaol.Power.BOUNCY: selection = blomkaol_2
		Blomkaol.Power.ROCKY: selection = blomkaol_3
		Blomkaol.Power.FLOATY: selection = blomkaol_4

	blomkaol_selection_visual.global_position = selection.global_position
	blomkaol_selection_visual.visible = true
