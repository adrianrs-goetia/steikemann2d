extends Object
class_name AudogFsm

var current_state: AudogState
var audog: AudogNode

func _init(init_owner: AudogNode) -> void:
	self.audog = init_owner
	current_state = AudogStateIdle.new(self.audog)

func integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	_process_state(current_state.integrate_forces(state))

func handle_bk_event(event: BlomkaolNode.Power) -> void:
	_process_state(current_state.handle_bk_event(event))

func handle_signal(what: AudogState.AudogEvent) -> void:
	_process_state(current_state.handle_signal(what))

func _process_state(new_state: AudogState) -> void:
	if new_state == null:
		return

	assert(current_state != new_state) # in case of state re-enter itself

	current_state.exit()
	current_state = new_state
	print(audog.name + " enter state: " + current_state.get_name())
	_process_state(current_state.enter())
