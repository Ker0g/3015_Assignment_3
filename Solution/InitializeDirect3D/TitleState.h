#pragma once
#include "State.h"
#include "SceneNode.h"

class TitleState : public State
{
public:
    TitleState(StateStack& stack, Context context);
    ~TitleState(); 

    virtual void    draw()                      override;
    virtual bool    update(const GameTimer& gt) override;
    virtual bool    handleInput()               override; 

private:
    RenderItem* mBackgroundSprite; 
    RenderItem* mText;            

    bool        mShowText;
    float       mTextEffectTime;
};
