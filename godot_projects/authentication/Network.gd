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
	while status != StreamPeerTCP.STATUS_CONNECTED:
		print(status)
		yield(get_tree().create_timer(1.0), "timeout")
		status = network_client.get_status()
		
		if status == StreamPeerTCP.STATUS_ERROR:
			print("Error connecting!")
	
	print("Connected to: " + network_client.get_connected_host())
	#client.big_endian = true;

func sendPacket(packet):
	network_client.put_data(packet.to_bytes());

func waitAuthenticationResponse():
	var waitingForResponse = true;
	while(waitingForResponse):
		var bytes_avail = network_client.get_available_bytes()
		var status_label = get_tree().get_root().find_node("Status", true, false)
		if bytes_avail > 0:
			var buffer = network_client.get_data(bytes_avail);
			print("Read " + str(bytes_avail) + " bytes")
			if buffer[0] == OK:
				var msgPacket = gameProto.Packet.new()
				var result_code = msgPacket.from_bytes(buffer[1]);
				if result_code == gameProto.PB_ERR.NO_ERRORS:
					if msgPacket.has_status():
						waitingForResponse = false;
						var errType = msgPacket.get_status().ErrorType
						match msgPacket.get_status().get_error():
							errType.OK:
								print("ok");
								status_label.text = "Logged in"
								get_tree().change_scene("res://Authentication.tscn");
							errType.Error:
								if msgPacket.get_status().has_error_string():
									print("ERROR")
									status_label.text = "Error: " + msgPacket.get_status().error_string()
								else:
									print("ERROR");
									status_label.text = "Error"
							errType.InvalidData:
								print("Invalid Data");
								status_label.text = "Invalid Data sent"
		else:
			yield(get_tree().create_timer(1.0), "timeout")

# Called when the node enters the scene tree for the first time.
func _ready():
	thread.start(self, "_handle_network", "localhost")

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
