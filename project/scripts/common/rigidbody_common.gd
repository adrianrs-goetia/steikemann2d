# Common RigidBody implementation across steikemann rigidbodies
# How to best deal with interface implementations in gdscript?

class_name RigidBodyCommon extends RigidBody3D

func on_pickup(socket: Node3D):
    pass

func on_throw(throw_impulse: Vector3):
    pass

func on_slam(...)
    pass

func attach_blomkaol(node: BlomkaolNode):
    pass

func detach_blomkaol():
    pass

func process_blomkaol_power(power: BlomkaolNode.Power):
    pass
