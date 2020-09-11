#include "Server.h"

Server::Server()
{

}

Server::~Server()
{
	delete instance;
}

Server* Server::instance = nullptr;

Server* Server::Instance()
{
	if (instance == nullptr)
	{
		instance = new Server();
	}
	else
	{
		return instance;
	}
}

void Server::start(asio::io_context& io_context,
	const tcp::endpoint& endpoint)
{
	tcp::acceptor p(io_context, endpoint);

	do_accept(p);
}

void Server::do_accept(tcp::acceptor& acc)
{
	std::shared_ptr<Session> temp;
	while (true)
	{
		temp = std::make_shared<Session>(acc.accept());
		sessions.insert(temp);
		temp.get()->start();
	}
}

void Server::endSession(std::shared_ptr<Session> &s)
{
	sessions.erase(s);
}

void Server::findRoom(std::shared_ptr<Session> &p)
{
	if (waitingRooms.size() == 0)
	{
		createRoom();
		std::cout << "Room created\n";
	}

	waitingRooms.begin()->get()->addPlayer(p);

	if (waitingRooms.begin()->get()->IsReady())
	{
		runningRooms.insert(*waitingRooms.begin());
		waitingRooms.erase(waitingRooms.begin());
		std::cout << "A new room is running\n";
	}
}

void Server::createRoom()
{
	waitingRooms.insert(std::make_shared<Room>());
}