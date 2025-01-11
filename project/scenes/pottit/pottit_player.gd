extends RigidBody3D

# enum STATE {
# 	idle = 0,
# 	running = 1,
# 	falling = 2
# }

# const MAX_SPEED: float = 6.5
# const ACCELERATION_SPEED: float = 40.0
# const DECELERATION_SPEED_GROUND: float = 30.0
# const DECELERATION_SPEED_AIR: float = 16.0
# const MIN_X_VELOCITY_STOP: float = MAX_SPEED / 5.0
# var recently_stopped: bool = false

# const JUMP_VELOCITY: float = 6.5
# const DOUBLE_JUMP_VELOCITY: float = 5.5
# const GRAVITY_SCALE_MAX: float = 1.7
# const GRAVITY_SCALE_DELTA_INCREASE: float = 2.2
# const GRAVITY_SCALE_BASE: float = 1.0
# # var gravity_scale: float = GRAVITY_SCALE_BASE
# const MAX_JUMPS: int = 2

# const LAND_MIN_VELOCITY_ROLL: float = MAX_SPEED / 3.0 # when to land standstill or with roll
# var land_blend_rolling: bool

# var state: STATE
# var pivot_dir: int
# var num_jumps: int

# func round_to_dec(num, digit) -> float:
# 	return round(num * pow(10.0, digit)) / pow(10.0, digit)

# func _ready() -> void:
# 	state = STATE.falling
# 	pivot_dir = -1 # facing right
# 	num_jumps = 0

# func _physics_process(delta: float) -> void:
# 	# Add the gravity.
# 	if not is_on_floor():
# 		gravity_scale = min(gravity_scale + delta * GRAVITY_SCALE_DELTA_INCREASE, GRAVITY_SCALE_MAX)
# 		velocity += Vector3(0, -9.81, 0) * gravity_scale * delta
# 		state = STATE.falling

# 	# Input
# 	var input_dir: Vector2 = Input.get_vector("move_left", "move_right", "ui_up", "ui_down", 0.05)
# 		var axis = Input.get_axis("move_left", "move_right")
# 	var direction: Vector2 = Vector2(input_dir.x, 0.0)
# 	if direction.x:
# 		velocity.x = move_toward(velocity.x, direction.x * MAX_SPEED, ACCELERATION_SPEED * delta)
# 		if recently_stopped:
# 			$pivot/AnimationTree.set("parameters/OneShot-StopRunning/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_ABORT)
# 		recently_stopped = false
# 		if not land_blend_rolling:
# 			$pivot/AnimationTree.set("parameters/OneShot-LandStill/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FADE_OUT)
# 	else:
# 		if state == STATE.running:
# 			velocity.x = move_toward(velocity.x, 0, DECELERATION_SPEED_GROUND * delta)
# 			if abs(velocity.x) > MIN_X_VELOCITY_STOP and not recently_stopped:
# 				recently_stopped = true
# 				$pivot/AnimationTree.set("parameters/OneShot-StopRunning/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE)
# 		elif state == STATE.falling:
# 			velocity.x = move_toward(velocity.x, 0, DECELERATION_SPEED_AIR * delta)
	
# 	velocity.x = round_to_dec(velocity.x, 1)
# 	var x: float = round_to_dec(velocity.x, 1)
# 	var abs_x: float = abs(x)

# 	# Landing
# 	match state:
# 		STATE.falling:
# 			if is_on_floor():
# 				state = STATE.running
# 				num_jumps = 0
# 				$pivot/AnimationTree.set("parameters/Blend-Falling/blend_amount", 0)
# 				land_blend_rolling = (abs(x) > LAND_MIN_VELOCITY_ROLL)
# 				$pivot/AnimationTree.set("parameters/Blend-Land/blend_amount", land_blend_rolling)
# 				$pivot/AnimationTree.set("parameters/OneShot-LandStill/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE)
# 			else:
# 				$pivot/AnimationTree.set("parameters/Blend-Falling/blend_amount", 0.85)
# 				$pivot/AnimationTree.set("parameters/Idle-Run/blend_amount", 0)
# 		STATE.running:
# 			gravity_scale = GRAVITY_SCALE_BASE
# 			$pivot/AnimationTree.set("parameters/Idle-Run/blend_amount", clamp(abs_x / MAX_SPEED, 0, 1))

# 	# Handle jump.
# 	if Input.is_action_just_pressed("jump") and num_jumps < MAX_JUMPS:
# 		if is_on_floor():
# 			$pivot/AnimationTree.set("parameters/OneShot-Jump/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE)
# 			num_jumps += 1
# 			velocity.y = JUMP_VELOCITY
# 		else:
# 			$pivot/AnimationTree.set("parameters/OneShot-Jump/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FADE_OUT)
# 			$pivot/AnimationTree.set("parameters/OneShot-DoubleJump/request", AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE)
# 			num_jumps = 2 # set to two in case of walking of an edge
# 			velocity.y = DOUBLE_JUMP_VELOCITY
# 		state = STATE.falling
# 		gravity_scale = GRAVITY_SCALE_BASE

# 	# Pivot facing direction
# 	if x != 0:
# 		if x > 0:
# 			pivot_dir = -1
# 		elif x < 0:
# 			pivot_dir = 1

# 	move_and_slide()
# 	position.z = 0

# func _process(_delta: float) -> void:
# 	$pivot.rotation.y = deg_to_rad(pivot_dir * -90)
