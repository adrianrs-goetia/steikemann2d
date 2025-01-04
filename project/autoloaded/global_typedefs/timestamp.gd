class_name Timestamp

var stamp: int

func timestamp() -> void:
    stamp = Time.get_ticks_msec()

# timeout in milliseconds
func timeout(time: int) -> bool:
    var t = Time.get_ticks_msec()
    var t2 = Time.get_ticks_msec()
    var diff_milli = (t - stamp)
    var diff_milli2 = (t2 - t)
    print("diff_milli: " + str(diff_milli) + ", time: " + str(time))
    print("diff_milli2: " + str(diff_milli2) + ", time: " + str(time))
    return diff_milli > time
