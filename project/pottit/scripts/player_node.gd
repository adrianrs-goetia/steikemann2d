extends RigidBody3D
class_name PlayerNode

var fsm: PlayerFsm
var model: PlayerModel
var blomkaol: Blomkaol
var gui: PlayerGui

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
	model.scale.x = -1

	blomkaol = $Blomkaol
	blomkaol.call_deferred("set_blomkaol_power", Blomkaol.Power.NONE)
	blomkaol.call_deferred("attach_to_player", self)

	gui = $PlayerGui
	gui.call_deferred("set_blomkaol_power", Blomkaol.Power.NONE)

	fsm = PlayerFsm.new(self)

func _exit_tree() -> void:
	InputContextHandler.erase_input_context_actions(PlayerInput.bindings)

func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed(PlayerInput.retract_blomkaol):
		blomkaol.attach_to_player(self)
	if event.is_action_pressed(PlayerInput.blomkaol_power_1):
		blomkaol.set_blomkaol_power(Blomkaol.Power.STICKY, gui)
	if event.is_action_pressed(PlayerInput.blomkaol_power_2):
		blomkaol.set_blomkaol_power(Blomkaol.Power.BOUNCY, gui)
	if event.is_action_pressed(PlayerInput.blomkaol_power_3):
		blomkaol.set_blomkaol_power(Blomkaol.Power.ROCKY, gui)
	if event.is_action_pressed(PlayerInput.blomkaol_power_4):
		blomkaol.set_blomkaol_power(Blomkaol.Power.FLOATY, gui)
	fsm.input(event)

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	fsm.integrate_forces(state)
