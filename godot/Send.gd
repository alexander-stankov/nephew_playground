extends Button

var file = preload("res://addressbook.gd");
var client = StreamPeerTCP.new();

# Called when the node enters the scene tree for the first time.
func _ready():
	var result = client.connect_to_host("127.0.0.1", 12349);
	if result != OK:
		print("Failure")
	else:
		print("Connected")

	while client.get_status() != StreamPeerTCP.STATUS_CONNECTED:
		print("Connecting...")

	client.big_endian = true;
	
	pass # Replace with function body.

func _pressed():
	var person = file.Person.new();
	person.set_id(1);
	person.set_name("Pesho");
	person.set_email("pesho@gmail.com");
	
	print(person.to_string());
	
	client.put_data(person.to_bytes());
	
#	var deserializedPerson = file.Person.new();
	
	#First example to get from the server
#
#	deserializedPerson.from_bytes(client.get_string().to_ascii());
#
#	print(deserializedPerson.to_string());
	
	#Second example
#	var data = client.get_data(30); #var data = client.get_data(person.to_bytes().size())
#	print(data.size());
#
#	deserializedPerson.from_bytes(data);
#	print(deserializedPerson.to_string());

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
