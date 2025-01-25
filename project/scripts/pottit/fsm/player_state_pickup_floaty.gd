extends PlayerState
class_name PlayerStatePickupFloaty

var move_horizontal = 0.0
var min_process_time = Timestamp.new()

func get_name() -> String:
    return "PickupFloaty"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    player.physics_material_override.friction = 0.0
    min_process_time.timestamp()
    player.model.in_air()
    return null

func exit() -> void:
    player.physics_material_override.friction = 1.0

func input(event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    if event.is_action_pressed(PlayerInput.pickup_throw):
        var other = player.pickup_socket.get_child(0) as AudogNode
        assert(other != null)
        other.on_throw(Vector3.ZERO)
        player.blomkaol.propogate_power.disconnect(player.process_bk_power)
        return PlayerStateInAir.new(move_horizontal)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    _set_movement_velocity(state.step)
    return null

func process_bk_power(power: BlomkaolNode.Power) -> PlayerState:
    match power:
        BlomkaolNode.Power.NONE:
            return PlayerStateInAir.new(move_horizontal)
    return null

##########################################################################
### Helper Functions
##########################################################################

func _set_movement_velocity(delta: float) -> void:
    var max_x = move_horizontal * Params.player_pickup_floaty_x_movement_max 
    var delta_x = delta * Params.player_pickup_floaty_x_movement_delta
    player.linear_velocity.x = _move_toward(max_x, delta_x)

    player.linear_velocity.y += Params.gravity * Params.player_pickup_floaty_float * delta
    player.linear_velocity.y = min(player.linear_velocity.y, Params.player_pickup_floaty_max_y_velocity)
