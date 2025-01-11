extends AudogState
class_name AudogThrownState

var impulse: Vector3

func _init(owner: AudogNode, what: AudogSignalThrown) -> void:
    super._init(owner)
    impulse = what.impulse

func get_name() -> String:
    return "Thrown"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.reparent(audog._original_parent)
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, false)
    audog.apply_impulse(impulse)
    
    if audog.blomkaol:
        return handle_bk_event(audog.blomkaol.current_power)
    else:
        return AudogIdleState.new(audog)

func exit() -> void:
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    pass
