extends RigidBody3D
class_name SmackableTest

var _blomkaol_state = Blomkaol.Power.NONE
const FLOATY = 100.0
const BOUNCY = 1.0
const MAX_BOUNCE_VELOCITY = 20.0
var collision_timestamp = Timestamp.new()

func process_blomkaol_power(delta: float, power: Blomkaol.Power):
	match power:
		Blomkaol.Power.NONE: _blomkaol_reset()
		Blomkaol.Power.FLOATY: _blomkaol_floaty(delta)
		Blomkaol.Power.BOUNCY: _blomkaol_bouncy()

func _blomkaol_reset():
	gravity_scale = 1
	_blomkaol_state = Blomkaol.Power.NONE
	contact_monitor = false

func _blomkaol_floaty(delta: float):
	_blomkaol_state = Blomkaol.Power.FLOATY
	gravity_scale = 1
	linear_velocity.y = FLOATY * delta

func _blomkaol_bouncy():
	_blomkaol_state = Blomkaol.Power.BOUNCY
	contact_monitor = true
	max_contacts_reported = 4

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	var pos = state.center_of_mass + position
	for i in state.get_contact_count():
		var velocity = state.get_contact_local_velocity_at_position(i) * -1

		DebugDraw3D.draw_line(pos, pos + velocity, Color.REBECCA_PURPLE, 0.2)
		
		match _blomkaol_state:
			Blomkaol.Power.BOUNCY:
				if collision_timestamp.timeout(100):
					collision_timestamp.timestamp()
					var bounce = min(velocity.length() * BOUNCY, MAX_BOUNCE_VELOCITY)
					linear_velocity = velocity.normalized() * bounce
					break

