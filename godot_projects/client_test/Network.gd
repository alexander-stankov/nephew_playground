extends Node

var network_client = StreamPeerTCP.new()
var thread = Thread.new()
var gameProto = preload("res://GameProtocol.gd")

# Thread must be disposed (or "joined"), for portability.
func _exit_tree():
	thread.wait_to_finish()

func _handle_network(_server_address):
	var result = network_client.connect_to_host("127.0.0.1", 12349);	

	if result != OK:
		print("Failure")
		return
	
	var status = network_client.get_status()
	var status_label = get_tree().get_root().find_node("StatusLabel", true, false)
	while status != StreamPeerTCP.STATUS_CONNECTED:
		status_label.text = "Connecting..."
		print(status)
		# Sleep for 1 second before next status check.
		# The timeout should be smaller but it will suffice for now
		yield(get_tree().create_timer(1.0), "timeout")
		status = network_client.get_status()
		
		if status == StreamPeerTCP.STATUS_ERROR:
			print("Error connecting!")
	
	print("Connected to: " + network_client.get_connected_host())
	status_label.text = "Connected to: " + network_client.get_connected_host() + ":" + str(network_client.get_connected_port())
	
	while true:
		var bytes_avail = network_client.get_available_bytes()
		if bytes_avail > 0:
			var buffer = network_client.get_data(bytes_avail);
			print("Read " + str(bytes_avail) + " bytes")
			
			if buffer[0] == OK:
				var msgPacket = gameProto.Packet.new()
				var result_code = msgPacket.from_bytes(buffer[1]);
				
				if result_code == gameProto.PB_ERR.NO_ERRORS:
					if msgPacket.has_status():
						var errType = msgPacket.get_status().ErrorType
						match msgPacket.get_status().get_error():
							errType.OK:
								status_label.text = "Logged in"
							errType.Error:
								if msgPacket.get_status().has_error_string():
									status_label.text = "Error: " + msgPacket.get_status().error_string()
								else:
									status_label.text = "Error"
							errType.InvalidData:
								status_label.text = "Invalid Data sent"
		else:
			yield(get_tree().create_timer(1.0), "timeout")
	#client.big_endian = true;

func send():
	var msgPacket = gameProto.Packet.new()
	var msgLogin = msgPacket.new_login()
	msgLogin.set_name("Alex")
	msgLogin.set_email("alex@testproto.com")
	msgLogin.set_password("secret_password")
	
	print("Sending")
	
	network_client.put_data(msgPacket.to_bytes());

# Called when the node enters the scene tree for the first time.
func _ready():
	# NOTE: this is NOT the way to do this, but since I don't care atm
	get_tree().get_root().find_node("Connect", true, false).connect("pressed", self, "send")
	thread.start(self, "_handle_network", "localhost")

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
