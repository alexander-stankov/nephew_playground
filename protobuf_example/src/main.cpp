
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include "game_protocol.pb.h"

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

			GameProtocol::Packet msgPacket;

			bool bRes = msgPacket.ParseFromString(buffer);

			if( !bRes )
			{
				std::cerr << "Error unknown message\n";
				//TODO(alex): return??
			}

			GameProtocol::Packet response;

			if( msgPacket.has_login() )
			{
				//We've got login packet
                std::cout << "Got login name: " << msgPacket.login().name()
                          << " email: " << msgPacket.login().email()
                          << " password: " << msgPacket.login().password();
				
				auto status = response.mutable_status();

				if (msgPacket.login().email() == "misho@gmail.com")
				{
					status->set_error(::GameProtocol::Status_ErrorType::Status_ErrorType_OK);
				}
				else
				{
					status->set_error(::GameProtocol::Status_ErrorType::Status_ErrorType_InvalidData);
				}
				
            }
			else if( msgPacket.has_register_() )
			{
				//We've got register packet
				std::cout << "Got register name: " << msgPacket.register_().name()
                          << " email: " << msgPacket.register_().email()
                          << " password: " << msgPacket.register_().password();

				auto status = response.mutable_status();
				status->set_error(::GameProtocol::Status_ErrorType::Status_ErrorType_OK);
			}
			else if( msgPacket.has_status() )
			{
				//We've got status packet
				switch( msgPacket.status().error() )
				{
					case GameProtocol::Status::ErrorType::Status_ErrorType_OK:
						std::cout << "Got status: OK\n";
						break;
                    case GameProtocol::Status::ErrorType::Status_ErrorType_Error:
						std::cout << "Got status: Error\n";
                        break;
                    case GameProtocol::Status::ErrorType::Status_ErrorType_InvalidData:
						std::cout << "Got status: Invalid Data\n";
                        break;
                }
				if( msgPacket.status().has_error_string() )
				{
					std::cerr << "Error message: " << msgPacket.status().error_string() << "\n";
				}
			}

			//Just to flush the stream
			std::cout << std::endl;

			std::string strResponse = response.SerializeAsString();
			//TODO(alex): error checking?

			asio::write(sock, asio::buffer(strResponse));
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
