#pragma once
#include "State.h"
#include "SceneNode.h"

class MenuState : public State
{
public:  
    
    enum OptionIndex
    {
        Play = 0,
        Exit = 1
    };

    MenuState(StateStack& stack, Context context);
    ~MenuState();

    virtual void draw() override;
    virtual bool update(const GameTimer& gt) override;
    virtual bool handleInput() override;

private:
    void updateOptionSelection();

private:
    RenderItem* mBackgroundSprite;          
    std::vector<RenderItem*> mOptions;    
    int         mOptionIndex;               
    bool        mEnterWasDown;             
    bool        mUpWasDown;                 
    bool        mDownWasDown;            
};


