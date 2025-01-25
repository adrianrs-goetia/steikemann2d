# Attack state is assumed to be oneshot
# Any data collected here is transient and is not relevant to subsequent attacks

extends PlayerState
class_name PlayerStateAttack

var move_horizontal = 0.0
var enter_time = Timestamp.new()
var attacked_instances: Array[int]
var has_attack_landed: bool = false # can apply blomkaol to one target

func get_name() -> String:
    return "Attack"

func _init(move_x: float) -> void:
    move_horizontal = move_x

func enter() -> PlayerState:
    enter_time.timestamp()
    player.physics_material_override.friction = 0.0

    player.model.bind_attack_collider_cb(Callable(self, "_on_attack"))
    player.model.oneshot_anim(PlayerModel.PlayerAnimOneShot.ATTACK)

    # attack collider checks physics layer player is on
    # add player here to make it ignore it in _on_attack
    attacked_instances.append(player.get_instance_id())

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

##########################################################################
### Helper Functions
##########################################################################

func _on_attack(body: Node3D):
    var id = body.get_instance_id()
    if attacked_instances.has(id):
        return
    attacked_instances.append(id)

    var rigidbody = body as RigidBody3D
    if rigidbody != null && !has_attack_landed:
        player.blomkaol.attach_to_other(rigidbody, Vector3(0,0,1))
    has_attack_landed = true
