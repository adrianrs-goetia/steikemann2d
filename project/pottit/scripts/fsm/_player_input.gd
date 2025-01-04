class_name PlayerInput

const left = "left"
const right = "right"
const jump = "jump"
const bindings: Dictionary = {
    left: [[InputType.KEY, KEY_A], [InputType.KEY, KEY_LEFT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, -1.0]],
    right: [[InputType.KEY, KEY_D], [InputType.KEY, KEY_RIGHT], [InputType.JOYPAD_MOTION, JOY_AXIS_LEFT_X, 1.0]],
    jump: [[InputType.KEY, KEY_SPACE], [InputType.KEY, KEY_UP], [InputType.JOYPAD_BUTTON, JOY_BUTTON_A]],
}