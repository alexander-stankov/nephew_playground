#include "Room.h"

Room::Room()
{

}

Room::~Room()
{

}

void Room::addPlayer(std::shared_ptr<Session>& s)
{
	players.insert(std::move(s));
}