# Template for further use
# Just copy this for each new fsm type...
# If it requires to know about the owners type, then a generic method is not that easy to do with gdscript

# extends Object
# class_name RigidBodyState # base abstract class

# func get_name() -> String:
#     assert(false)
#     return ""

# func can_enter() -> bool:
#     return true

# func enter() -> RigidBodyState:
#     return null

# func exit() -> void:
#     pass

# func integrate_forces(_state: PhysicsDirectBodyState3D) -> RigidBodyState:
#     return null
