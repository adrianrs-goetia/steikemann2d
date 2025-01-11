extends Object
class_name AubergineDogFsm

var current_state: AubergineDogState
var owner: AubergineDogNode

func _init(init_owner: AubergineDogNode) -> void:
    self.owner = init_owner
    current_state.player = self.owner

func input(event: InputEvent) -> void:
    _process_state(current_state.input(event))

func integrate_forces(state: PhysicsDirectBodyState3D):
    _process_state(current_state.integrate_forces(state))

func _process_state(new_state: AubergineDogState) -> void:
    if new_state == null:
        return

    # Will allow for state to re-enter itself
    current_state.exit()
    current_state =	new_state
    current_state.player = self.owner
    _process_state(current_state.enter())
