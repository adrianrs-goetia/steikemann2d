extends AudogState
class_name AudogIdleState

func get_name() -> String:
    return "Idle"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.mass = 1
    audog.lock_rotation = false
    audog.freeze = false
    audog.gravity_scale = 1
    audog.contact_monitor = false
    audog.max_contacts_reported = 0
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    
    return null

func exit() -> void:
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AudogState:
    return null
