extends Object
class_name PlayerState

var player: PlayerNode

func get_name() -> String:
    assert(false)
    return ""

func can_enter() -> bool:
    return true

func enter() -> PlayerState:
    return null

func exit() -> void:
    pass

func input(_event: InputEvent) -> PlayerState:
    return null

func integrate_forces(_state: PhysicsDirectBodyState3D) -> PlayerState:
    return null
