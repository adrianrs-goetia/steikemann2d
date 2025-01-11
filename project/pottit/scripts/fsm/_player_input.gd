class_name PlayerInput

const left = "left"
const right = "right"
const jump = "jump"
const attack = "attack"
const retract_blomkaol = "retract_blomkaol"
const blomkaol_power_1 = "blomkaol_power_1"
const blomkaol_power_2 = "blomkaol_power_2"
const blomkaol_power_3 = "blomkaol_power_3"
const blomkaol_power_4 = "blomkaol_power_4"
const bindings: Dictionary = {
    left: [[InputType.KEY, KEY_A], [InputType.KEY, KEY_LEFT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, -1.0]],
    right: [[InputType.KEY, KEY_D], [InputType.KEY, KEY_RIGHT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, 1.0]],
    jump: [[InputType.KEY, KEY_SPACE], [InputType.KEY, KEY_UP], [InputType.JOYPAD_BUTTON, JOY_BUTTON_A]],
    attack: [[InputType.KEY, KEY_U], [InputType.MOUSE_BUTTON, MOUSE_BUTTON_LEFT], [InputType.JOYPAD_BUTTON, JOY_BUTTON_X]],
    retract_blomkaol: [[InputType.KEY, KEY_F], [InputType.JOYPAD_BUTTON, JOY_BUTTON_B]],
    blomkaol_power_1: [[InputType.KEY, KEY_1], [InputType.JOYPAD_BUTTON, JOY_BUTTON_DPAD_UP]],
    blomkaol_power_2: [[InputType.KEY, KEY_2], [InputType.JOYPAD_BUTTON, JOY_BUTTON_DPAD_RIGHT]],
    blomkaol_power_3: [[InputType.KEY, KEY_3], [InputType.JOYPAD_BUTTON, JOY_BUTTON_DPAD_DOWN]],
    blomkaol_power_4: [[InputType.KEY, KEY_4], [InputType.JOYPAD_BUTTON, JOY_BUTTON_DPAD_LEFT]],
}