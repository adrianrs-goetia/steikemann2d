extends AudogState
class_name AudogStatePickedUp

var socket: Node3D

func _init(owner: AudogNode, what: AudogEventPickedUp) -> void:
    super._init(owner)
    socket = what.socket_node

func get_name() -> String:
    return "PickedUp"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.reparent(socket)
    audog.global_position = socket.global_position
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, false)
    audog.freeze = true
    return null

func exit() -> void:
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    audog.freeze = false
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AudogState:
    return null

func handle_bk_event(_event: BlomkaolNode.Power) -> AudogState:
    return null