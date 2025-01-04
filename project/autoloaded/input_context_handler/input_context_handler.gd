@tool
extends Node

# Expects Array[Array[int]]
func get_events(array) -> Array[InputEvent]:
	var arr: Array[InputEvent]
	for element in array:
		match element[0]:
			InputType.KEY:
				var event = InputEventKey.new()
				event.keycode = element[1]
				arr.append(event)
			InputType.MOUSE_BUTTON:
				var event = InputEventMouseButton.new()
				event.button_index = element[1]
				arr.append(event)
			InputType.JOYPAD_BUTTON:
				var event = InputEventJoypadButton.new()
				event.button_index = element[1]
				arr.append(event)
			InputType.JOYPAD_MOTION:
				var event = InputEventJoypadMotion.new()
				event.axis = element[1]
				event.axis_value = element[2]
				arr.append(event)
	return arr

# var example_input_actions: Dictionary = {
# 	"a": [[InputType.MOUSE_BUTTON, MOUSE_BUTTON_LEFT], [InputType.JOYPAD_BUTTON, JOY_BUTTON_A]],
# 	"left": [[InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, -1.0], [InputType.KEY, KEY_A]],
# 	"right": [[InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, 1.0], [InputType.KEY, KEY_D]],
# }

func _enter_tree() -> void:
	print("input_map _enter_tree")
	set_process_shortcut_input(true)
	set_process_input(true)
	process_mode = Node.PROCESS_MODE_ALWAYS

	# register_input_context_actions(example_input_actions)
	# add_action_events(example_input_actions)

func register_input_context_actions(actions: Dictionary):
	for action in actions:
		InputMap.add_action(action)

func erase_input_context_actions(actions: Dictionary):
	for action in actions:
		InputMap.action_erase_events(action)

func add_action_events(actions: Dictionary):

	for action in actions.keys():
		var dictentry = actions[action]
		print(str(action) + ", " + str(dictentry))

		for event in get_events(dictentry):
			InputMap.action_add_event(action, event)
