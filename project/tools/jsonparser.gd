@tool
extends Node

func _enter_tree() -> void:
    set_process_input(true)
    set_process_unhandled_input(true)

func _input(event: InputEvent) -> void:
    if event.is_action_pressed("0"):
        print("NEW INPUT ACTION HANDLED \"0\"")
    if event.is_action_pressed("a"):
        print("NEW INPUT ACTION HANDLED \"a\"")
    if event.is_action_pressed("m"):
        print("NEW INPUT ACTION HANDLED \"m\"")

