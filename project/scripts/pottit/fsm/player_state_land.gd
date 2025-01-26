extends PlayerState
class_name PlayerStateLand

var enter_time = Timestamp.new()

func get_name() -> String:
    return "Land"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    enter_time.timestamp()
    player.physics_material_override.friction = 0.0
    player.model.oneshot_anim(PlayerModel.PlayerAnimOneShot.LAND)

    if player.pickup_socket.get_child_count():
        return PlayerStatePickupOnGround.new(move_horizontal, player.pickup_socket.get_child(0) as AudogNode)
    
    return PlayerStateOnGround.new(move_horizontal)

func exit() -> void:
    pass

func input(_event: InputEvent) -> PlayerState:
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    return null
