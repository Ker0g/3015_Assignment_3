#pragma once
#include "Command.h"
#include <map>

class CommandQueue;

class Player
{
	enum Action
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		ActionCount
	};

public:
	Player();
	void handleEvent(CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void	assignKey(Action action, char key);
	int getAssignedKey(Action action) const;

private:
	void	initActions();
	static bool isRealtimeAction(Action action);

private:
	std::map<int, Action>		mKeyBinding;   

	std::map<Action, Command>	mActionBinding;

	std::map<int, bool>			mKeyFlag;    

};

