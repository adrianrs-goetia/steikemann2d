@tool
extends Node

var default_input_actions: Dictionary = {
	KEY_0: "0",
	KEY_A: "a",
	KEY_M: "m",
}

func _enter_tree() -> void:
	print("input_map _enter_tree")
	set_process_shortcut_input(true)

	# Always register default actions
	register_input_context_actions(default_input_actions)
	add_action_events(default_input_actions)

func _shortcut_input(event: InputEvent) -> void:
	if event is InputEventKey:
		handle_input_event_key(event)

func handle_input_event_key(event: InputEventKey) -> void:
	if event.is_action_pressed("ui_down"):
		print("erasing actions")
		for keycode in default_input_actions.keys():
			var action = default_input_actions[keycode]
			print(str(keycode) + ", " + str(action))
			InputMap.action_erase_events(action)
	elif event.is_action_pressed("ui_up"):
		print("inserting actions")
		add_action_events(default_input_actions)

func register_input_context_actions(actions: Dictionary):
	for keycode in actions:
		var action = actions[keycode]
		InputMap.add_action(action)

func erase_input_context_actions(actions: Dictionary):
	for keycode in actions:
		var action = actions[keycode]
		InputMap.action_erase_events(action)

func add_action_events(actions: Dictionary):
	for keycode in actions.keys():
		var action = actions[keycode]
		print(str(keycode) + ", " + str(action))

		var event = InputEventKey.new()
		event.keycode = keycode
		InputMap.action_add_event(action, event)