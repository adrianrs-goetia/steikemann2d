extends Node3D

var PlayerNode: CharacterBody3D
const X_ACCELERATION_LERP: float = 3.4
const X_LERP_MAX_DIST: float = 8 # Max distance in front of the player
const MIN_Y_HEIGHT: float = -3

func _ready() -> void:
	var node: Node = get_node('/root/Main/Pottit_Player')
	PlayerNode = node

func _physics_process(delta: float) -> void:
	position.z = 0
	var x_player = PlayerNode.position.x
	var x_lerp: float = x_player
	x_lerp += PlayerNode.velocity.x * X_ACCELERATION_LERP
	x_lerp = clamp(x_lerp, x_player - X_LERP_MAX_DIST, x_player + X_LERP_MAX_DIST)

	var input_dir: Vector2 = Input.get_vector("move_left", "move_right", "ui_up", "ui_down", 0.05)
	if input_dir.x or not PlayerNode.is_on_floor():
		position.x = lerp(position.x, x_lerp, delta)
	position.y = max(lerp(position.y, PlayerNode.position.y, delta), MIN_Y_HEIGHT)
