extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	var temp = get_tree().get_root().get_node("Network")
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_Register_pressed():
	get_tree().change_scene("res://RegisterScene.tscn");

func _on_Login_pressed():
	get_tree().change_scene("res://LoginScene.tscn");
