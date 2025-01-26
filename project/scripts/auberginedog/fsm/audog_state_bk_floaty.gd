extends AudogState
class_name AudogStateBkFloaty

func get_name() -> String:
    return "BkFloaty"

func can_enter() -> bool:
    return true

func enter() -> AudogState:
    audog.gravity_scale = 0
    return null

func exit() -> void:
    audog.gravity_scale = 1

func integrate_forces(state: PhysicsDirectBodyState3D) -> AudogState:
    state.apply_central_force(Vector3(0, Params.audog_bk_floaty_force * state.step, 0))
    return null
