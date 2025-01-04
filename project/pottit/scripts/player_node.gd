extends RigidBody3D
class_name PlayerNode

var fsm: PlayerFsm

func _enter_tree() -> void:
    InputContextHandler.register_input_context_actions(PlayerInput.bindings)
    InputContextHandler.add_action_events(PlayerInput.bindings)

    lock_rotation = true
    contact_monitor = true
    max_contacts_reported = 4
    can_sleep = false
    gravity_scale = 0
    axis_lock_linear_z = true
    position.z = 0 # sanity

    fsm = PlayerFsm.new(self)

func _unhandled_input(event: InputEvent) -> void:
    fsm.input(event)

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
    fsm.integrate_forces(state)
