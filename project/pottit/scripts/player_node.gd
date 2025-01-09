extends RigidBody3D
class_name PlayerNode

var fsm: PlayerFsm
var model: PlayerModel

func _enter_tree() -> void:
	InputContextHandler.add_action_events(PlayerInput.bindings)

	lock_rotation = true
	contact_monitor = true
	max_contacts_reported = 4
	can_sleep = false
	gravity_scale = 0 # each state applies gravity separately
	axis_lock_linear_z = true
	position.z = 0 # sanity

	model = $Model
	model.rotation_degrees.y = Params.player_model_rotation_angle # ->
	fsm = PlayerFsm.new(self)
	model.scale.x = -1

func _exit_tree() -> void:
	InputContextHandler.erase_input_context_actions(PlayerInput.bindings)

func _unhandled_input(event: InputEvent) -> void:
	fsm.input(event)

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	fsm.integrate_forces(state)
