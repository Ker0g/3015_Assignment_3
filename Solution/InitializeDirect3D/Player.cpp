#include "Player.h"
#include "CommandQueue.h"
#include "Aircraft.h"
#include "Command.h" 

struct AircraftMover
{
    AircraftMover(float vx, float vy, float vz) :
        velocity(vx, vy, vz)
    {
    }

    void operator() (Aircraft& aircraft, const GameTimer&) const
    {
        aircraft.accelerate(velocity);
    }

    XMFLOAT3 velocity;
};

Player::Player()
{
	mKeyBinding[VK_LEFT] = MoveLeft;
	mKeyBinding[VK_RIGHT] = MoveRight;
	mKeyBinding[VK_UP] = MoveUp;
	mKeyBinding[VK_DOWN] = MoveDown;

	initActions();

	for (auto pair : mKeyBinding)
	{
		mKeyFlag[pair.first] = false;
	}

	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (!isRealtimeAction(pair.second))
		{
			if (mKeyFlag[pair.first])
			{
				if (!GetAsyncKeyState(pair.first))
				{
					mKeyFlag[pair.first] = false;
				}
			}
			else
			{
				if (GetAsyncKeyState(pair.first) & 0x8000)
				{
					mKeyFlag[pair.first] = true;
					commands.push(mActionBinding[pair.second]);
				}
			}
		}
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (GetAsyncKeyState(pair.first) & 0x8000 && isRealtimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, char key)
{
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	mKeyBinding[key] = action;
}

int Player::getAssignedKey(Action action) const  
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}
	return 0x00;
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
		return true;

	default:
		return false;
	}
}

void Player::initActions()
{
	const float playerSpeed = 2.0f;

	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, 0.0f, +playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, 0.0f, -playerSpeed));
}
