extends AudogState
class_name AuDogStateBkStickyStuck

func get_name() -> String:
    return "BkStickyStuck"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.freeze = true
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    return null

func exit() -> void:
    audog.freeze = false
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AudogState:
    return null
