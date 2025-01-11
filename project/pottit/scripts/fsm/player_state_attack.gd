extends PlayerState
class_name PlayerStateAttack

var move_horizontal = 0.0
var enter_time = Timestamp.new()

func get_name() -> String:
    return "Attack"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    enter_time.timestamp()
    player.physics_material_override.friction = 0.0

    player.model.bind_attack_collider_cb(Callable(self, "_on_attack"))
    player.model.oneshot_anim(PlayerModel.PlayerAnimOneShot.ATTACK)

    return null

func exit() -> void:
    player.model.unbind_attack_collider_cb(Callable(self, "_on_attack"))

func input(_event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    if enter_time.timeout(Params.player_attack_time):
        if _ground_directly_below(state.center_of_mass, Params.player_ground_check_raycast_length):
            return PlayerStateOnGround.new(move_horizontal)
        else:
            return PlayerStateInAir.new(move_horizontal)

    var delta = state.step
    player.linear_velocity.x = _move_toward(move_horizontal, delta)
    player.linear_velocity.y -= Params.gravity * Params.player_gravity_scale * delta

    return null

func _on_attack(body: Node3D):
    var rigidbody = body as RigidBody3D
    if rigidbody != null:
        # var dir = _x_direction(player.global_position, body.global_position)
        # rigidbody.linear_velocity = Vector3(dir * 6, 5, 0)
        player.blomkaol.attach_to_other(rigidbody, Vector3(0,0,1))
