#pragma once
#include "State.h"
#include "SceneNode.h"

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context);
    ~PauseState();

    virtual void    draw()                      override;
    virtual bool    update(const GameTimer& gt) override;
    virtual bool    handleInput()               override;

private:
    RenderItem* mOverlay;
    RenderItem* mPausedText;
    RenderItem* mInstructionText;
    bool        mEscapeWasDown;
    bool        mBackspaceWasDown;
};