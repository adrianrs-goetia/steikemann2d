extends Object
class_name AudogState # base abstract class

class AudogEvent extends Object: # abstract event
    func _init() -> void:
        assert(false)
        pass

class AudogEventPickedUp extends AudogEvent:
    func _init(node: Node3D) -> void:
        self.socket_node = node
    var socket_node: Node3D

class AudogEventThrown extends AudogEvent:
    func _init(thrown_impulse: Vector3) -> void:
        self.impulse = thrown_impulse
    var impulse: Vector3

class AudogEventSlammed extends AudogEvent:
    func _init(curve: MovementCurve) -> void:
        self.slam_curve = curve
    var slam_curve: MovementCurve

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
        BlomkaolNode.Power.NONE: return AudogStateIdle.new(audog)
        BlomkaolNode.Power.STICKY: return AudogStateBkStickyFree.new(audog)
        BlomkaolNode.Power.FLOATY: return AudogStateBkFloaty.new(audog)
        BlomkaolNode.Power.BOUNCY: return AudogStateBkBouncy.new(audog)
        BlomkaolNode.Power.ROCKY: return AudogStateBkRocky.new(audog)
    return null

func handle_signal(what: AudogEvent) -> AudogState:
    if what is AudogEventThrown:
        return AudogStateThrown.new(audog, what)

    if what is AudogEventPickedUp:
        return AudogStatePickedUp.new(audog, what)
    
    if what is AudogEventSlammed:
        return AudogStateSlammed.new(audog, what)

    return null
