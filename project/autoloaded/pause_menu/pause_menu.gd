extends Control

@export var level_selection_scene: PackedScene

const escape = "escape"
const quit = "quit"
var input_context: Dictionary = {
	quit: [[InputType.KEY, KEY_ESCAPE], [InputType.JOYPAD_BUTTON, JOY_BUTTON_BACK]],
	escape: [[InputType.KEY, KEY_P], [InputType.JOYPAD_BUTTON, JOY_BUTTON_START]]
}

# var input_context: Dictionary = {
# 	quit: [[InputType.KEY, KEY_ESCAPE], [InputType.JOYPAD_BUTTON, JOY_BUTTON_BACK]],
# 	escape: [[InputType.KEY, KEY_P], [InputType.JOYPAD_BUTTON, JOY_BUTTON_START]]
# }


var is_paused: bool
var gui_stack: Array[Control] = []

var focus = 0

func _enter_tree() -> void:
	is_paused = false
	visible = is_paused
	InputContextHandler.add_action_events(input_context)

	# default gui stack
	gui_stack.append($MainScreen)
	$BackButton.visible = false

	# Set button callbacks
	var resume_button = $MainScreen/Resume
	var exit_button = $MainScreen/Quit
	var back_button = $BackButton/Button
	resume_button.pressed.connect(_toggle_pause)
	exit_button.pressed.connect(_exit)
	back_button.pressed.connect(_pop_stack)
	if level_selection_scene != null:
		var level_select_button = $MainScreen/LevelSelect
		var _call = func(): _append_stack(level_selection_scene)
		level_select_button.pressed.connect(_call)

func _exit_tree() -> void:
	InputContextHandler.erase_input_context_actions(input_context)

func _input(event: InputEvent) -> void:
	if event.is_action_pressed(escape):
		_toggle_pause()
	if event.is_action_pressed(quit):
		_exit()

func _toggle_pause() -> void:
	is_paused = !is_paused
	get_tree().paused = is_paused
	_set_paused_status(is_paused)

func _overwrite_pause(paused: bool) -> void:
	is_paused = paused
	get_tree().paused = is_paused
	_set_paused_status(is_paused)

func _set_paused_status(paused: bool) -> void:
	visible = paused
	if !paused:
		while gui_stack.size() > 1:
			_pop_stack()

func _exit():
	get_tree().quit(0)

func _append_stack(packedscene: PackedScene) -> void:
	if packedscene == null:
		printerr("PauseMenu, tried to append empty packedscene to stack")
		return

	var scene = packedscene.instantiate() as Control
	gui_stack.append(scene)
	add_child(scene)

	if gui_stack.size() > 1:
		$BackButton.visible = true
		$MainScreen.visible = false

func _pop_stack() -> void:
	# Do not pop the last stack as that is the MainScreen
	if gui_stack.size() == 1:
		return

	var last = gui_stack.pop_back() as Control
	remove_child(last)
	last.queue_free()

	if gui_stack.size() == 1:
		$BackButton.visible = false
		$MainScreen.visible = true
