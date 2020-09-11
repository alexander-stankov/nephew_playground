#include "Session.h"
#include "Server.h"

#include <iostream>

Session::Session()
	: socket_(nullptr)
{

}

Session::Session(tcp::socket &socket)
	: socket_(std::move(socket))
{
	//start();
}

Session::~Session()
{

}

void Session::start()
{
	receive();
}

void Session::receive()
{
	try
	{
		while(true)
		{
			asio::error_code error;

			char buffer[1024] = { 0 };

			size_t length = socket_.receive(asio::buffer(buffer));

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

			if (!bRes)
			{
				std::cerr << "Error unknown message\n";
				//TODO(alex): return??
			}

			GameProtocol::Packet response;

			if (msgPacket.has_login())
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
			else if (msgPacket.has_register_())
			{
				//We've got register packet
				std::cout << "Got register name: " << msgPacket.register_().name()
					<< " email: " << msgPacket.register_().email()
					<< " password: " << msgPacket.register_().password();

				auto status = response.mutable_status();
				status->set_error(::GameProtocol::Status_ErrorType::Status_ErrorType_OK);
			}
			else if (msgPacket.has_status())
			{
				//We've got status packet
				switch (msgPacket.status().error())
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
				if (msgPacket.status().has_error_string())
				{
					std::cerr << "Error message: " << msgPacket.status().error_string() << "\n";
				}
			}

			//Just to flush the stream
			std::cout << std::endl;

			send(response);

			//Testing the function without an appropriate Protobuf message
			joinRoom();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void Session::send(const GameProtocol::Packet &msg)
{
	std::string strResponse = msg.SerializeAsString();
	//TODO(alex): error checking?

	asio::write(socket_, asio::buffer(strResponse));
}

void Session::end()
{
	socket_.close();
	Server::Instance()->endSession(shared_from_this());
}

void Session::joinRoom()
{
	Server::Instance()->findRoom(shared_from_this());
}