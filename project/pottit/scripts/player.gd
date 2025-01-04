extends RigidBody3D
class_name Player

class PlayerInput:
    const left = "left"
    const right = "right"
    const bindings: Dictionary = {
        left: [[InputType.KEY, KEY_A], [InputType.KEY, KEY_LEFT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, -1.0]],
        right: [[InputType.KEY, KEY_D], [InputType.KEY, KEY_RIGHT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, 1.0]],
    }

class PlayerMovementCache:
    var horizontal = 0.0

var movement = PlayerMovementCache.new()

func _enter_tree() -> void:
    InputContextHandler.register_input_context_actions(PlayerInput.bindings)
    InputContextHandler.add_action_events(PlayerInput.bindings)

    lock_rotation = true
    contact_monitor = true
    max_contacts_reported = 4
    can_sleep = false

func _unhandled_input(event: InputEvent) -> void:
    movement.horizontal = Input.get_axis(PlayerInput.left, PlayerInput.right)

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
    for i in state.get_contact_count():
        var normal = state.get_contact_local_normal(i)
        var pos = state.get_contact_local_position(i)
        DebugDraw3D.draw_line(pos, pos + normal, Color.BLUE, state.step)
        if normal.dot(Globals.up) > Params.player_floor_angle:
            linear_velocity.x = movement.horizontal * state.step * Params.player_move_speed
            toggle_friction()
        else:
            physics_material_override.friction = 0.0

    
    position.z = 0 # sanity adjustment

func toggle_friction() -> void:
    if abs(movement.horizontal) > 0.0:
        physics_material_override.friction = 0.0
    else:
        physics_material_override.friction = 1.0