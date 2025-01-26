# Helper class to map elapsed time over a specified duration to 0-1
# Mapping elapsed time to the X-axis of a curve
class_name CurveTime

var elapsed_time: float = 0.0
var duration: float

func _init(_duration: float):
    duration = _duration

func step(delta: float) -> float:
    elapsed_time = clamp(elapsed_time + delta, 0, duration)
    return elapsed_time / duration

func done() -> bool:
    return duration <= elapsed_time
