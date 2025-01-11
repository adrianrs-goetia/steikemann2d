# extends Object
# class_name RigidBodyFsm

# var current_state: RigidBodyState
# var owner: RigidBody3D

# func _init(init_owner: RigidBody3D) -> void:
#     self.owner = init_owner
#     current_state.player = self.owner

# func input(event: InputEvent) -> void:
#     _process_state(current_state.input(event))

# func integrate_forces(state: PhysicsDirectBodyState3D):
#     _process_state(current_state.integrate_forces(state))

# func _process_state(new_state: RigidBodyState) -> void:
#     if new_state == null:
#         return

#     # Will allow for state to re-enter itself
#     current_state.exit()
#     current_state =	new_state
#     current_state.player = self.owner
#     _process_state(current_state.enter())
