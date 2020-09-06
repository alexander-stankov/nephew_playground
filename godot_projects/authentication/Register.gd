extends Button

var username = "";
var email = "";
var password = "";
var confirmPassword = "";

var gameProto = preload("res://GameProtocol.gd")

# Called when the node enters the scene tree for the first time.
func _ready():
	
	pass # Replace with function body.

func _on_ConfirmPasswordText_text_changed(new_text):
	confirmPassword = new_text;

func _on_UsernameText_text_changed(new_text):
	username = new_text;

func _on_EmailText_text_changed(new_text):
	email = new_text;

func _on_PasswordText_text_changed(new_text):
	password = new_text;

func _on_Register_pressed():
	#Here should be more checks about the uniqueness of the account and whether the email is correct
	
	if(password != confirmPassword):
		print("Confirm your password first");
		return;
	
	var msgPacket = gameProto.Packet.new();
	var msgRegister = msgPacket.new_register();
	
	msgRegister.set_name(username);
	msgRegister.set_email(email);
	msgRegister.set_password(password);
	
	get_node("/root/Network").sendPacket(msgPacket);
	get_node("/root/Network").waitAuthenticationResponse();

func setStatusLabel(text):
	var status_label = get_tree().get_root().find_node("Status", true, false)
	status_label.text = text;
