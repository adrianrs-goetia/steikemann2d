class_name Globals
const up = Vector3(0, 1, 0)
const forward = Vector3(1, 0, 0)
const depth = Vector3(0, 0, -1)

enum COLLISION {
    # STATIC_WORLD    = 1 << 1,
    # DYNAMIC_WORLD   = 1 << 1,
    # PLAYER          = 1 << 2,
    # ENEMIES         = 1 << 3,
    STATIC_WORLD    = 1,
    DYNAMIC_WORLD   = 2,
    PLAYER          = 3,
    ENEMIES         = 4,
}