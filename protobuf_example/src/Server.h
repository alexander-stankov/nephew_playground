#pragma once

#include <set>
#include <memory>
#include <utility>

#include "asio.hpp"
#include "Session.h"
#include "Room.h"

using asio::ip::tcp;

class Server
{
public:
	~Server();

	void start(asio::io_context& io_context,
		const tcp::endpoint& endpoint);
	void endSession(std::shared_ptr<Session> &s);
	void findRoom(std::shared_ptr<Session>&);

	static Server* Instance();
private:
	Server();
	Server(Server const& map);

	void do_accept(tcp::acceptor &);
	void createRoom();

	static Server* instance;
	std::set<std::shared_ptr<Session>> sessions;
	std::set<std::shared_ptr<Room>> runningRooms;
	std::set<std::shared_ptr<Room>> waitingRooms;
};