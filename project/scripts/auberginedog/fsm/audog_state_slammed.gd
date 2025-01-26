extends AudogState
class_name AudogStateSlammed

var slam_curve: MovementCurve

func _init(owner: AudogNode, what: AudogEventSlammed) -> void:
    super._init(owner)
    slam_curve = what.slam_curve

func get_name() -> String:
    return "Slammed"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.reparent(audog._original_parent)

    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, false)
    audog.set_collision_mask_value(Globals.COLLISION.DYNAMIC_WORLD, false)

    return null

func exit() -> void:
    audog.set_collision_mask_value(Globals.COLLISION.PLAYER, true)
    audog.set_collision_mask_value(Globals.COLLISION.DYNAMIC_WORLD, true)
    pass

func integrate_forces(state: PhysicsDirectBodyState3D) -> AudogState:
    audog.global_position = slam_curve.process(state.step)
    if slam_curve.done():
        return AudogStateIdle.new(audog)
    return null
