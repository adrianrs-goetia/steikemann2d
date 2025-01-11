extends RigidBody3D
class_name AubergineDogNode

var _blomkaol_state = Blomkaol.Power.NONE
var _original_parent: Node
var _collision_layer_cache: int
var _picked_up: bool

func _ready() -> void:
	_original_parent = get_parent()

func _enter_tree() -> void:
	_blomkaol_reset()

func on_throw(throw_impulse: Vector3):
	_picked_up = false
	reparent(_original_parent)
	freeze = false
	collision_layer = _collision_layer_cache
	apply_impulse(throw_impulse)

func on_pickup(socket: Node3D):
	_picked_up = true
	reparent(socket)
	_collision_layer_cache = collision_layer
	collision_layer = 0
	global_position = socket.global_position
	freeze = true

func process_blomkaol_power(delta: float, power: Blomkaol.Power):
	if _picked_up:
		return
	match power:
		Blomkaol.Power.NONE: _blomkaol_reset()
		Blomkaol.Power.STICKY: _blomkaol_sticky()
		Blomkaol.Power.FLOATY: _blomkaol_floaty(delta)
		Blomkaol.Power.BOUNCY: _blomkaol_bouncy()
		Blomkaol.Power.ROCKY: _blomkaol_rocky()

func _blomkaol_reset():
	mass = 1
	lock_rotation = false
	freeze = false
	gravity_scale = 1
	_blomkaol_state = Blomkaol.Power.NONE
	contact_monitor = false
	max_contacts_reported = 0

var _sticky_enter = Timestamp.new()
const STICKY_COLLISION_WAIT_TIME = 300 # time before sticky collision is even checked
enum {
	STICKY_FREE,
	STICKY_STUCK,
}
var _sticky_state: int
func _blomkaol_sticky():
	if _sticky_state == STICKY_STUCK:
		freeze = true
		return
	else:
		freeze = false
	_blomkaol_state = Blomkaol.Power.STICKY
	_sticky_state = STICKY_FREE
	contact_monitor = true
	max_contacts_reported = 4
	_sticky_enter.timestamp()

const FLOATY = 100.0
func _blomkaol_floaty(delta: float):
	freeze = false
	_blomkaol_state = Blomkaol.Power.FLOATY
	gravity_scale = 0
	apply_central_force(Vector3(0, FLOATY * delta, 0))

func _blomkaol_rocky():
	freeze = false
	_blomkaol_state = Blomkaol.Power.ROCKY
	gravity_scale = 2
	mass = 1
	lock_rotation = true

const BOUNCY = 1.0
const MAX_BOUNCE_VELOCITY = 10.0
const BOUNCE_COLLISION_WAIT_TIME = 100 # time between each collision handling
var _bouncy_collision = Timestamp.new()
func _blomkaol_bouncy():
	_blomkaol_state = Blomkaol.Power.BOUNCY
	contact_monitor = true
	max_contacts_reported = 4

func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	var pos = state.center_of_mass + position
	for i in state.get_contact_count():
		match _blomkaol_state:
			Blomkaol.Power.BOUNCY:
				if _bouncy_collision.timeout(BOUNCE_COLLISION_WAIT_TIME):
					var velocity = state.get_contact_local_velocity_at_position(i) * -1
					DebugDraw3D.draw_line(pos, pos + velocity, Color.REBECCA_PURPLE, 0.2)
					_bouncy_collision.timestamp()
					var bounce = min(velocity.length() * BOUNCY, MAX_BOUNCE_VELOCITY)
					linear_velocity = velocity.normalized() * bounce
					break
			Blomkaol.Power.STICKY:
				# if _sticky_enter.timeout(STICKY_COLLISION_WAIT_TIME) and _sticky_state == STICKY_FREE:
				if _sticky_state == STICKY_FREE:
					_sticky_state = STICKY_STUCK
					freeze = true
					break
