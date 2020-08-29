#include <google/protobuf/stubs/common.h>
#include <iostream>
#include "addressbook.pb.h"
#include "../3rd_party/asio/asio.hpp"

using asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
	try
	{
		for (;;)
		{
			asio::error_code error;
			char bytes[max_length];

			size_t length = sock.read_some(asio::buffer(bytes), error);
			if (error == asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw asio::system_error(error);// Some other error.

			std::cout << "Received chars: " << bytes << std::endl << "Length: " << length << std::endl;

			tutorial::Person person;
			person.ParseFromString(bytes);

			std::cout << person.name();
			std::cout << person.email();
			std::cout << person.id() << std::endl;

			std::string s;

			//person.SerializeToArray(bytes, length);

			person.SerializeToString(&s);

			std::cout << "Serialized:" << s;

			asio::write(sock, asio::buffer(s));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void server(asio::io_context& io_context, unsigned short port)
{
	tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		std::thread(session, a.accept()).detach();
	}
}

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: " << argv[0] << " <port>\n";
			//return 1;
		}

		asio::io_context io_context;

		server(io_context, 12349); //std::atoi(argv[1])
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	google::protobuf::ShutdownProtobufLibrary();
	system("pause");

	return 0;
}