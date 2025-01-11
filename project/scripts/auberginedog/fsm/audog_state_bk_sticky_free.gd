extends AudogState
class_name AudogBkStickyFreeState


func get_name() -> String:
    return "BkStickyFree"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.contact_monitor = true
    audog.max_contacts_reported = 4
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, false)
    audog.freeze = false
    return null

func exit() -> void:
    audog.contact_monitor = false
    audog.max_contacts_reported = 0

func integrate_forces(state: PhysicsDirectBodyState3D) -> AudogState:
    # Assume collisions within the remaining physics 
    # layers are desired
    for i in state.get_contact_count():
        return AuDogBkStickyStuckState.new(audog)
    return null
