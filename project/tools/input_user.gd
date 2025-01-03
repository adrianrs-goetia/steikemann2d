class_name InputUser extends Node

var input_actions: Dictionary = {
	KEY_K: "k",
	KEY_A: "attack",
	KEY_L: "l",
}

func _enter_tree() -> void:
	set_process_unhandled_input(true)
	InputContextHandler.register_input_context_actions(input_actions)
	InputContextHandler.add_action_events(input_actions)

func _exit_tree() -> void:
	InputContextHandler.erase_input_context_actions(input_actions)

func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("attack"):
		print("Input used action pressed \"attack\"")
	if event.is_action_pressed("k"):
		print("Input used action pressed \"k\"")
	if event.is_action_pressed("l"):
		print("Input used action pressed \"l\"")
