
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include "addressbook.pb.h"

//NOTE: So we've already have include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/asio/ )
// why do you include this like this and not just #include "asio.hpp"??
//#include "../3rd_party/asio/asio.hpp"

#include "asio.hpp"

using asio::ip::tcp;

//NOTE: this one should be constexpr. It is evaluated at compile time and has other advantages
//also it is better for this kind of constants to be UPPER_CASE e.g MAX_LENGTH.
//also what is MAX_LENGTH? the length of a string maybe. In this use case it should be BUFFER_SIZE
//const int max_length = 1024;
constexpr int BUFFER_SIZE = 1024;

void session(tcp::socket sock)
{
	try
	{
		//NOTE: isn't while(true) more clear???
		for (;;)
		{
			asio::error_code error;
			//NOTE: bytes is a bad name. It should be more descriptive.
			//in this case this is a buffer used to send/receive messages so buffer
			//would be better name. Also always initialize stack allocated arrays,
			//because otherwise they have undefined value e.g. not zero
			//char bytes[max_length];
            char buffer[BUFFER_SIZE] = {0};

            //size_t length = sock.read_some(asio::buffer(buffer), error);
            size_t length = sock.receive(asio::buffer(buffer));

			//NOTE: its better to have braces ALWAYS even for one-liners
			if (error == asio::error::eof)
			{
				break; // Connection closed cleanly by peer.
			}
			else if (error)
			{
				throw asio::system_error(error);// Some other error.
			}

			//NOTE: we don't want to use std::endl at all because it flushes the stream e.g. has performance implications
			//use "\n" instead
			std::cout << "Received chars: " << buffer << "\n" << "Length: " << length << "\n";

			tutorial::Person person;
			person.ParseFromString(buffer);
			//perse.ParseFrom

			std::cout << person.name();
			std::cout << person.email();
			std::cout << person.id() << std::endl;

			std::string s;

			//person.SerializeToArray(bytes, length);

			person.SerializeToString(&s);

			std::cout << "Serialized:" << s << "\n";

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

	//NOTE: isn't while(true) more clear???
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
		//NOTE: so we're terminating if the user hasn't entered argument
		//but discarding the argument afterwards????

		// if (argc != 2)
		// {
		// 	std::cerr << "Usage: " << argv[0] << " <port>\n";
		// 	//return 1;
		// }

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
