extends Object
class_name PlayerFsm

var current_state: PlayerState

# state bank
var state_onground = PlayerStateOnGround.new()
var state_jump = PlayerStateJump.new()
var state_inair = PlayerStateInAir.new()

func _init(player: PlayerNode) -> void:
    state_onground.init(player)
    state_inair.init(player)
    state_jump.init(player)

    current_state = state_onground

func input(event: InputEvent) -> void:
    _process_state(current_state.input(event))

func integrate_forces(state: PhysicsDirectBodyState3D):
    _process_state(current_state.integrate_forces(state))

func _process_state(new_state: PlayerState.Type) -> void:
    if new_state == PlayerState.Type.NONE:
        return
    
    # Will allow for state to re-enter itself
    current_state.exit()
    var s = ""
    match new_state:
        PlayerState.Type.ONGROUND:
            current_state = state_onground
            s += "OnGround"
        PlayerState.Type.JUMP:
            current_state = state_jump
            s += "Jump"
        PlayerState.Type.INAIR:
            current_state = state_inair
            s += "InAir"
    print("PlayerFsm enter state: " + s)
    _process_state(current_state.enter())
