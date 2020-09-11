
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include "game_protocol.pb.h"

#include "asio.hpp"
#include "Server.h"

using asio::ip::tcp;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	try
	{
		asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), 12349);
		
		Server::Instance()->start(io_context, endpoint);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	google::protobuf::ShutdownProtobufLibrary();
	system("pause");

	return 0;
}