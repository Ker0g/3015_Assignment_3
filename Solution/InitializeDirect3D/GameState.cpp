#include "GameState.h"
#include "Application.h"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(context.app)
	, mPlayer(*context.player)
	, mEscWasDown(true) 
{
	mWorld.buildScene();
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(const GameTimer& gt)
{
	mWorld.update(gt);
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	return true;
}

bool GameState::handleInput()
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(commands);

    bool escDown = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
	if (escDown && !mEscWasDown)
	{
		mEscWasDown = true;
		requestStackPush(States::Pause);
	}
    else if (!escDown)
        mEscWasDown = false;

    return true;
}

GameState::~GameState()
{
}