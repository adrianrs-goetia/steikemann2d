extends AudogState
class_name AudogStateBkRocky

func get_name() -> String:
    return "BkRocky"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.gravity_scale = 2
    audog.mass = 1.4
    audog.lock_rotation = true
    return null

func exit() -> void:
    audog.gravity_scale = 1
    audog.mass = 1.0
    audog.lock_rotation = false
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AudogState:
    return null
