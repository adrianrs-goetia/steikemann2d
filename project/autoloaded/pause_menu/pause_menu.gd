extends Control

const escape = "escape"
const quit = "quit"
var input_context: Dictionary = {
	quit: [[InputType.KEY, KEY_ESCAPE], [InputType.JOYPAD_BUTTON, JOY_BUTTON_BACK]],
	escape: [[InputType.KEY, KEY_P], [InputType.JOYPAD_BUTTON, JOY_BUTTON_START]]
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
	if event.is_action_pressed(quit):
		_exit()

func _toggle_pause():
	is_paused = !is_paused
	get_tree().paused = is_paused
	visible = is_paused

func _exit():
	get_tree().quit(0)
