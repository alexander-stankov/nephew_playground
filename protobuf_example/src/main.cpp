#include <google/protobuf/stubs/common.h>
#include <iostream>
#include "addressbook.pb.h"

void CreateAddress(tutorial::Person* person)
{
    int id = 1;
    person->set_id( id );

	std::string name = "Test user";
	person->set_name(name);

	std::string email = "email@test.com";
	person->set_email( email );
}

void PrintAdressBook(const tutorial::AddressBook& book)
{
	std::string delimiter(20, '-');
	std::cout << delimiter << "\n";

	auto people = book.people();
	for( auto person : people )
	{
		std::cout << "ID: " << person.id() << "\n";
		std::cout << "Name: " << person.name() << "\n";
		std::cout << "Email: " << person.email() << "\n";
	}

	std::cout << delimiter << "\n";
}

int main( int argc, char** argv )
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	tutorial::AddressBook address_book;
	tutorial::AddressBook deserialized_book;

	CreateAddress(address_book.add_people());

	std::cout << "\nBefore serialization:\n";
	PrintAdressBook(address_book);

	std::string serialized;
	address_book.SerializeToString(&serialized);

	if( deserialized_book.ParseFromString(serialized) )
	{
		std::cout << "\nAfter deserialization:\n";
		PrintAdressBook(deserialized_book);
	}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
