extends AudogState
class_name AudogStateBkBouncy

var bounce_collision = Timestamp.new()

func get_name() -> String:
    return "BkBouncy"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.contact_monitor = true
    audog.max_contacts_reported = 4
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, false)
    return null

func exit() -> void:
    audog.contact_monitor = false
    audog.max_contacts_reported = 0
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    pass

func integrate_forces(state: PhysicsDirectBodyState3D) -> AudogState:
    var pos = audog.global_position
    for i in state.get_contact_count():
        if bounce_collision.timeout(Params.audog_bk_bouncy_collision_interval):
            bounce_collision.timestamp()
            var velocity = state.get_contact_local_velocity_at_position(i) * -1
            DebugDraw3D.draw_line(pos, pos + velocity, Color.REBECCA_PURPLE, 0.2)
            var bounce = min(velocity.length() * Params.audog_bk_bouncy_strength, Params.audog_bk_bouncy_max_velocity)
            audog.linear_velocity = velocity.normalized() * bounce
            break

    return null
