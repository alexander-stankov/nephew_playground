#pragma once

#include "asio.hpp"
#include "game_protocol.pb.h"

using asio::ip::tcp;

class Session
	: public std::enable_shared_from_this<Session>
{
public:
	Session();
	Session(tcp::socket &socket);
	~Session();
	
	void start();

private:
	void receive();
	void send(const GameProtocol::Packet& msg);
	void end();
	void joinRoom();

	tcp::socket socket_;
};