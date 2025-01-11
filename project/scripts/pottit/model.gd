extends Node3D
class_name PlayerModel

var _animation_tree: AnimationTree
var _speed: float = 0.0
var _oneshot_fadeout_timer: Timer
var _attack_collider: Area3D

func _ready() -> void:
	_animation_tree = $AnimationTree
	_attack_collider = $BoneAttachment3D/AttackCollider

	_oneshot_fadeout_timer = Timer.new()
	_oneshot_fadeout_timer.autostart = false
	add_child(_oneshot_fadeout_timer)
	_oneshot_fadeout_timer.connect("timeout", _oneshot_fadeout)

###########################################
### Animations
###########################################
func set_idle_run_blend(speed: float):
	_speed = move_toward(_speed, speed, get_physics_process_delta_time() * Params.player_idle_run_blend_speed)
	_animation_tree.set("parameters/BlendSpace1D/blend_position", _speed / Params.player_move_speed)
	
func on_ground():
	_animation_tree.set("parameters/Transition/transition_request", "IdleRun")
	
func in_air():
	_animation_tree.set("parameters/Transition/transition_request", "InAir")

enum PlayerAnimOneShot {
	JUMP,
	LAND,
	ATTACK,
}
func oneshot_anim(oneshot: PlayerAnimOneShot):
	var statemachine: AnimationNodeStateMachinePlayback = _animation_tree.get("parameters/OneShotStateMachine/playback")
	_oneshot_abort()
	var fadeout_time: float
	match oneshot:
		PlayerAnimOneShot.JUMP:
			statemachine.travel("Jump")
			fadeout_time = 0.9
		PlayerAnimOneShot.LAND:
			statemachine.travel("Land")
			fadeout_time = 0.7
		PlayerAnimOneShot.ATTACK:
			statemachine.travel("Attack")
			fadeout_time = 0.5
	
	_animation_tree.set("parameters/OneShot/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE)
	_animation_tree.set("fadeout_time", fadeout_time)
	_oneshot_fadeout_timer.start(fadeout_time)

func _oneshot_abort():
	_oneshot_fadeout_timer.stop()
	_animation_tree.set("parameters/OneShot/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_ABORT)

func _oneshot_fadeout():
	_animation_tree.set("parameters/OneShot/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FADE_OUT)


###########################################
### Collisions
###########################################
func bind_attack_collider_cb(cb: Callable):
	_attack_collider.monitoring = true
	_attack_collider.connect("body_entered", cb)
func unbind_attack_collider_cb(cb: Callable):
	_attack_collider.monitoring = false
	_attack_collider.disconnect("body_entered", cb)
