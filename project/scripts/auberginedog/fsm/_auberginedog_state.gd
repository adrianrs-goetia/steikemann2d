extends Object
class_name AubergineDogState # base abstract class

func get_name() -> String:
    assert(false)
    return ""

func can_enter() -> bool:
    return true

func enter() -> AubergineDogState:
    return null

func exit() -> void:
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AubergineDogState:
    return null
