#pragma once
#include "../../Common/GameTimer.h"
#include <memory>
class StateStack;
class Player;
class Application;

namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Game,
		Pause, 
		Loading,
	};
}

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(Application* app, Player* player)
			: app(app)
			, player(player)
		{
		}

		Application* app;
		Player* player;
	};


public:
	State(StateStack& stack, Context context);
	virtual				~State(); 
	virtual void		draw() = 0;
	virtual bool		update(const GameTimer& gt) = 0;
	virtual bool        handleInput() = 0;


protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

private:
	StateStack* mStack;
	Context				mContext;
};
