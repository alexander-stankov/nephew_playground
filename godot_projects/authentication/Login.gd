extends Button

var email = "";
var password = "";
var gameProto = preload("res://GameProtocol.gd");

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _on_PasswordText_text_changed(new_text):
	password = new_text;

func _on_EmailText_text_changed(new_text):
	email = new_text;


func _on_Login_pressed():
	var msgPacket = gameProto.Packet.new();
	var msgLogin = msgPacket.new_login();
	
	msgLogin.set_email(email);
	msgLogin.set_password(password);
	#Lets keep this way for now till we decide whether to use email or username
	msgLogin.set_name("misho");
		
	get_node("/root/Network").sendPacket(msgPacket);
	get_node("/root/Network").waitAuthenticationResponse();
