extends Label3D

@onready
var area = $Area3D

func _enter_tree() -> void:
	visible = false


func _on_area_3d_body_entered(body: Node3D) -> void:
	var player = body as PlayerCharacterBody
	if player:
		visible = true
