# Slamming a pickup will launch the player on top of it
# Based on the pickup and blomkaol powers on it, the player
# will automatically move into land_* states based on 
# blomkaol powers on the pickup

extends PlayerState
class_name PlayerStatePickupSlam

var slam_enemy_curve: MovementCurve = preload("res://resources/player_slam/enemy_slammed_curve.tres") as MovementCurve
var slam_jump_curve: MovementCurve = preload("res://resources/player_slam/player_slam_jump_curve.tres") as MovementCurve
var audog: AudogNode

func get_name() -> String:
    return "PickupSlam"

func _init(move_x: float, other: AudogNode) -> void:
    move_horizontal = move_x
    audog = other

func enter() -> PlayerState:
    player.physics_material_override.friction = 0.0
    # player.model.in_air() #todo: new anim

    var enemy_start = player.pickup_socket.global_position
    var enemy_end = player.global_position + Vector3(_get_forward_x(), 0, 0)
    slam_enemy_curve.init(enemy_start, enemy_end)
    audog.on_slam(slam_enemy_curve)

    var player_end = enemy_end + (Vector3.UP)
    slam_jump_curve.init(player.global_position, player_end)
    
    return null

func exit() -> void:
    player.physics_material_override.friction = 1.0

func input(_event: InputEvent) -> PlayerState:
    move_horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)
    return null

func integrate_forces(state: PhysicsDirectBodyState3D) -> PlayerState:
    # todo: player position along slam
    player.global_position = slam_jump_curve.process(state.step)

    if slam_jump_curve.done():
        return PlayerStateLand.new(move_horizontal)
    
    return null

func process_bk_power(_power: BlomkaolNode.Power) -> PlayerState:
    return null
    # return _process_bk_power_default(power)

##########################################################################
### Helper Functions
##########################################################################

func _collide_with_slammed_pickup(state: PhysicsDirectBodyState3D) -> PlayerState:
    for i in player.landing.get_collision_count():
        var other = player.landing.get_collider(i)

        var audog = other as AudogNode
        if audog:
            match player.blomkaol.get_power_on_other(audog):
                BlomkaolNode.Power.NONE:
                    return PlayerStateLand.new(move_horizontal)
                # BlomkaolNode.Power.STICKY:
                # BlomkaolNode.Power.BOUNCY:
                # BlomkaolNode.Power.ROCKY:
                # BlomkaolNode.Power.FLOATY:
        
    return null
