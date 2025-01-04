extends Control
class_name LevelSelection

const path = "res://levels/"

func _enter_tree() -> void:
	print("LevelSelection _enter_tree")
	var level_files: PackedStringArray = DirAccess.open(path).get_files()

	var vbox: VBoxContainer = $LevelSelection/VBox
	assert(vbox != null)

	for child in vbox.get_children():
		child.queue_free()

	for level in level_files:
		var button = Button.new()
		button.text = level
		vbox.add_child(button)
		var _call = func(): _change_scene(path + level)
		button.pressed.connect(_call)

func _change_scene(scenepath: String) -> void:
	print("Change Scene: " + scenepath)
	get_tree().call_deferred("change_scene_to_file", scenepath)
	var pausemenu = get_node("/root/PauseMenu") as PauseMenu
	pausemenu._overwrite_pause(false)
