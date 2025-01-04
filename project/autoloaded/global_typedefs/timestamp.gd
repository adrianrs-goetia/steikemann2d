class_name Timestamp

var stamp: int

func timestamp() -> void:
    stamp = Time.get_ticks_msec()

# timeout in milliseconds
func timeout(time: int) -> bool:
    var t = Time.get_ticks_msec()
    var diff_milli = (t - stamp)
    return diff_milli > time
