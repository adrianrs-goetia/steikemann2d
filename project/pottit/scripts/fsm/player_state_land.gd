extends PlayerState
class_name PlayerStateLand

var move_horizontal = 0.0
var enter_time = Timestamp.new()

func get_name() -> String:
    return "Land"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    enter_time.timestamp()
    player.physics_material_override.friction = 0.0
    player.model.oneshot_anim(PlayerModel.PlayerAnimOneShot.LAND)
    return null

func exit() -> void:
    pass

func input(_event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    if enter_time.timeout(Params.player_jump_time):
        return PlayerStateOnGround.new(move_horizontal)

    var delta = state.step
    player.linear_velocity.x = _move_toward(move_horizontal, delta)
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * delta

    return null
