#pragma once

#include <utility>
#include <memory>

#include "Session.h"

class Room
{
public:
	Room();
	~Room();

	void addPlayer(std::shared_ptr<Session> &s);
	inline bool IsReady() { return players.size() == MAX_PLAYERS ? true : false; }

private:
	std::set<std::shared_ptr<Session>> players;
	const int MAX_PLAYERS = 1;
};

