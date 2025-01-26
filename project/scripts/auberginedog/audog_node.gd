extends RigidBody3D
class_name AudogNode

var _original_parent: Node

var blomkaol: BlomkaolNode = null
var fsm: AudogFsm

func _ready() -> void:
    _original_parent = get_parent()
    fsm = AudogFsm.new(self)

func on_throw(throw_impulse: Vector3):
    fsm.handle_signal(AudogState.AudogEventThrown.new(throw_impulse))

func on_pickup(socket: Node3D):
    fsm.handle_signal(AudogState.AudogEventPickedUp.new(socket))

## TODO:: on_slam
func on_slam(curve: MovementCurve):
    fsm.handle_signal(AudogState.AudogEventSlammed.new(curve))

func attach_blomkaol(node: BlomkaolNode):
    blomkaol = node
    blomkaol.propogate_power.connect(process_blomkaol_power)

func detach_blomkaol():
    blomkaol.propogate_power.disconnect(process_blomkaol_power)
    blomkaol = null
    fsm.handle_bk_event(BlomkaolNode.Power.NONE) # todo: use fsm_event instead?

func process_blomkaol_power(power: BlomkaolNode.Power):
    fsm.handle_bk_event(power) # todo: as fsm_event?

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
    fsm.integrate_forces(state)
