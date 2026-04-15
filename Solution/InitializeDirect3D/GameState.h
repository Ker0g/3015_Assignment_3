#pragma once

#include "State.h"
#include "SceneNode.h"
#include "World.h"
#include "Player.h"

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void draw() override;
    virtual bool update(const GameTimer& gt) override;
    virtual bool handleInput()override; 
    ~GameState();

private:
    World   mWorld;
    Player& mPlayer;  
    bool    mEscWasDown;
};