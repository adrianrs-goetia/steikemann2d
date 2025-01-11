extends Object
class_name AudogState # base abstract class

class AudogSignal extends Object: # abstract
    func _init() -> void:
        assert(false)
        pass
class AudogSignalPickedUp extends AudogSignal:
    func _init(node: Node3D) -> void:
        self.socket_node = node
    var socket_node: Node3D
class AudogSignalThrown extends AudogSignal:
    func _init(thrown_impulse: Vector3) -> void:
        self.impulse = thrown_impulse
    var impulse: Vector3

var audog: AudogNode

func _init(owner: AudogNode):
    audog = owner

func get_name() -> String:
    assert(false)
    return ""

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    return null

func exit() -> void:
    pass

func integrate_forces(_state: PhysicsDirectBodyState3D) -> AudogState:
    return null

func handle_bk_event(event: BlomkaolNode.Power) -> AudogState:
    match event:
        BlomkaolNode.Power.NONE: return AudogIdleState.new(audog)
        BlomkaolNode.Power.STICKY: return AudogBkStickyFreeState.new(audog)
        BlomkaolNode.Power.FLOATY: return AudogBkFloatyState.new(audog)
        BlomkaolNode.Power.BOUNCY: return AudogBkBouncyState.new(audog)
        BlomkaolNode.Power.ROCKY: return AudogBkRockyState.new(audog)
    return null

func handle_signal(what: AudogSignal) -> AudogState:
    if what is AudogSignalThrown:
        return AudogThrownState.new(audog, what)

    if what is AudogSignalPickedUp:
        return AudogPickedUpState.new(audog, what)

    return null
