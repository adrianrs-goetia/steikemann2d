extends Object
class_name PlayerFsm

var current_state: PlayerState
var owner: PlayerNode

func _init(player: PlayerNode) -> void:
    self.owner = player
    current_state = PlayerStateOnGround.new(0.0)
    current_state.player = self.owner

func input(event: InputEvent) -> void:
    _process_state(current_state.input(event))

func integrate_forces(state: PhysicsDirectBodyState3D) -> void:
    _process_state(current_state.integrate_forces(state))

func process_bk_power(power: BlomkaolNode.Power) -> void:
    _process_state(current_state.process_bk_power(power))

func _process_state(new_state: PlayerState) -> void:
    if new_state == null:
        return

    # Will allow for state to re-enter itself
    current_state.exit()
    current_state =	new_state
    current_state.player = self.owner
    print("PlayerFsm enter state: " + current_state.get_name())
    _process_state(current_state.enter())
