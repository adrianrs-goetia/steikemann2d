extends Control

const escape = "escape"
var input_context: Dictionary = {
	KEY_ESCAPE: escape,
	KEY_P: escape
}

var is_paused: bool

func _enter_tree() -> void:
	is_paused = false
	visible = is_paused
	InputContextHandler.register_input_context_actions(input_context)
	InputContextHandler.add_action_events(input_context)

	var resume_button = $VBoxContainer/Button
	var exit_button = $VBoxContainer/Button2
	resume_button.pressed.connect(_toggle_pause)
	exit_button.pressed.connect(_exit)

func _input(event: InputEvent) -> void:
	if event.is_action_pressed(escape):
		_toggle_pause()

func _toggle_pause():
	is_paused = !is_paused
	get_tree().paused = is_paused
	visible = is_paused

func _exit():
	get_tree().quit(0)