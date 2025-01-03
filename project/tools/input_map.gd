@tool
extends Node

var input: Dictionary = {
	"pressed": false,
}

var input_dict: Dictionary = {
	KEY_0: input,
	KEY_A: input,
	KEY_M: input,
}

var default_input_actions: Array = [
	"0",
	"a",
	"m"
]

var input_action_mapping: Dictionary = {
	KEY_0: default_input_actions[0],
	KEY_A: default_input_actions[1],
	KEY_M: default_input_actions[2],
}

func _enter_tree() -> void:
	print("input_map _enter_tree")
	set_process_shortcut_input(true)
	register_input_context_actions(default_input_actions)
	insert_action_events()

func _shortcut_input(event: InputEvent) -> void:
	if event is InputEventKey:
		handle_input_event_key(event)

func handle_input_event_key(event: InputEventKey) -> void:
	if event.is_action_pressed("ui_down"):
		print("erasing actions")
		for keycode in input_action_mapping.keys():
			var action = input_action_mapping[keycode]
			print(str(keycode) + ", " + str(action))
			InputMap.action_erase_events(action)
	elif event.is_action_pressed("ui_up"):
		print("inserting actions")
		insert_action_events()

func register_input_context_actions(actions: Array):
	for action in actions:
		InputMap.add_action(action)

func insert_action_events():
	for keycode in input_action_mapping.keys():
		var action = input_action_mapping[keycode]
		print(str(keycode) + ", " + str(action))

		var event = InputEventKey.new()
		event.keycode = keycode
		InputMap.action_add_event(action, event)