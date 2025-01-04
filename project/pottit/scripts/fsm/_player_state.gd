extends Object
class_name PlayerState

enum Type {
    NONE,
    ONGROUND,
    JUMP,
    INAIR,
}

var player: PlayerNode

func init(owner: PlayerNode) -> void:
    self.player = owner

func can_enter() -> bool:
    return true

func enter() -> Type:
    return Type.NONE

func exit():
    pass

func input(_event: InputEvent) -> Type:
    return Type.NONE

func integrate_forces(_state: PhysicsDirectBodyState3D) -> Type:
    return Type.NONE
