extends Button


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

#That is the way to do it
func _pressed():
	get_node("/root/Network").send();
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
