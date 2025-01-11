extends RigidBody3D
class_name AudogNode

var _original_parent: Node

var blomkaol: BlomkaolNode = null
var fsm: AudogFsm

func _ready() -> void:
    _original_parent = get_parent()
    fsm = AudogFsm.new(self)

func on_throw(throw_impulse: Vector3):
    fsm.handle_signal(AudogState.AudogSignalThrown.new(throw_impulse))

func on_pickup(socket: Node3D):
    fsm.handle_signal(AudogState.AudogSignalPickedUp.new(socket))

func attach_blomkaol(node: BlomkaolNode):
    blomkaol = node
func detach_blomkaol():
    blomkaol = null
    fsm.handle_bk_event(BlomkaolNode.Power.NONE) # todo: use signal instead?

func process_blomkaol_power(power: BlomkaolNode.Power):
    fsm.handle_bk_event(power) # todo: as signal?

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
    fsm.integrate_forces(state)
