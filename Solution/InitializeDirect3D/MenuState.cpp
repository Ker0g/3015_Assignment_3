#include "MenuState.h"
#include "Application.h"
#include "StateStack.h"
#include <algorithm>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
    , mBackgroundSprite(nullptr)
    , mOptionIndex(Play)
    , mEnterWasDown(false)
    , mUpWasDown(false)
    , mDownWasDown(false)
{
    Application* app = getContext().app;

    auto bgRitem = std::make_unique<RenderItem>();
    mBackgroundSprite = bgRitem.get();
    XMMATRIX bgScale = XMMatrixScaling(5.8f, 4.35f, 1.f);
    XMMATRIX bgRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX bgTranslate = XMMatrixTranslation(-2.9f, 2, 2.2f);
    XMStoreFloat4x4(&mBackgroundSprite->World, bgScale * bgRotate * bgTranslate);
    XMStoreFloat4x4(&mBackgroundSprite->TexTransform, XMMatrixScaling(1.f, 1.f, 1.f));
    mBackgroundSprite->ObjCBIndex = (UINT)app->getRenderItems().size();
    mBackgroundSprite->Mat = app->getMaterials()["TitleScreen"].get();
    mBackgroundSprite->Geo = app->getGeometries()["shapeGeo"].get();
    mBackgroundSprite->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mBackgroundSprite->IndexCount = mBackgroundSprite->Geo->DrawArgs["quad"].IndexCount;
    mBackgroundSprite->StartIndexLocation = mBackgroundSprite->Geo->DrawArgs["quad"].StartIndexLocation;
    mBackgroundSprite->BaseVertexLocation = mBackgroundSprite->Geo->DrawArgs["quad"].BaseVertexLocation;
    app->mRitemLayer[(int)Application::RenderLayer::Opaque].push_back(bgRitem.get());
    app->getRenderItems().push_back(std::move(bgRitem));

    auto playRitem = std::make_unique<RenderItem>();
    XMMATRIX optionScale = XMMatrixScaling(1.5, 0.4f, 1.f);
    XMMATRIX optionRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX playTranslate = XMMatrixTranslation(-1, 2.1f, 0.5f);
    XMStoreFloat4x4(&playRitem->World, optionScale * optionRotate * playTranslate);
    XMStoreFloat4x4(&playRitem->TexTransform, XMMatrixScaling(1, 1, 1));
    playRitem->ObjCBIndex = (UINT)app->getRenderItems().size();
    playRitem->Mat = app->getMaterials()["Play"].get();
    playRitem->Geo = app->getGeometries()["shapeGeo"].get();
    playRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    playRitem->IndexCount = playRitem->Geo->DrawArgs["quad"].IndexCount;
    playRitem->StartIndexLocation = playRitem->Geo->DrawArgs["quad"].StartIndexLocation;
    playRitem->BaseVertexLocation = playRitem->Geo->DrawArgs["quad"].BaseVertexLocation;
    mOptions.push_back(playRitem.get());
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(playRitem.get());
    app->getRenderItems().push_back(std::move(playRitem));

    auto exitRitem = std::make_unique<RenderItem>();
    XMMATRIX exitTranslate = XMMatrixTranslation(-1, 2.1f, 0.0f);
    XMStoreFloat4x4(&exitRitem->World, optionScale * optionRotate * exitTranslate);
    XMStoreFloat4x4(&exitRitem->TexTransform, XMMatrixScaling(1, 1, 1));
    exitRitem->ObjCBIndex = (UINT)app->getRenderItems().size();
    exitRitem->Mat = app->getMaterials()["Exit"].get();
    exitRitem->Geo = app->getGeometries()["shapeGeo"].get();
    exitRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    exitRitem->IndexCount = exitRitem->Geo->DrawArgs["quad"].IndexCount;
    exitRitem->StartIndexLocation = exitRitem->Geo->DrawArgs["quad"].StartIndexLocation;
    exitRitem->BaseVertexLocation = exitRitem->Geo->DrawArgs["quad"].BaseVertexLocation;
    mOptions.push_back(exitRitem.get());
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(exitRitem.get());
    app->getRenderItems().push_back(std::move(exitRitem));

    updateOptionSelection();
}

void MenuState::draw()
{
}

bool MenuState::update(const GameTimer& gt)
{
    return true;
}

bool MenuState::handleInput()
{
    bool upDown = GetAsyncKeyState(VK_UP) & 0x8000;
    if (upDown && !mUpWasDown)
    {
        mUpWasDown = true;
        if (mOptionIndex > 0)
            mOptionIndex--;
        else
            mOptionIndex = (int)mOptions.size() - 1;
        updateOptionSelection();
    }
    else if (!upDown)
        mUpWasDown = false;

    bool downDown = GetAsyncKeyState(VK_DOWN) & 0x8000;
    if (downDown && !mDownWasDown)
    {
        mDownWasDown = true;
        if (mOptionIndex < (int)mOptions.size() - 1)
            mOptionIndex++;
        else
            mOptionIndex = 0;
        updateOptionSelection();
    }
    else if (!downDown)
        mDownWasDown = false;

    bool enterDown = GetAsyncKeyState(VK_RETURN) & 0x8000;
    if (enterDown && !mEnterWasDown)
    {
        mEnterWasDown = true;
        if (mOptionIndex == Play)
        {
            requestStackPop();
            requestStackPush(States::Game);
        }
        else if (mOptionIndex == Exit)
        {
            requestStackPop();  
            PostQuitMessage(0);
        }
    }
    else if (!enterDown)
        mEnterWasDown = false;

    return false;
}

void MenuState::updateOptionSelection()
{
    for (int i = 0; i < (int)mOptions.size(); ++i)
    {
        if (i == mOptionIndex)
            mOptions[i]->Mat->DiffuseAlbedo = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
        else
            mOptions[i]->Mat->DiffuseAlbedo = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

        mOptions[i]->Mat->NumFramesDirty = gNumFrameResources;
    }
}

MenuState::~MenuState()
{
    Application* app = getContext().app;

    auto& opaqueLayer = app->mRitemLayer[(int)Application::RenderLayer::Opaque];
    opaqueLayer.erase(
        std::remove(opaqueLayer.begin(), opaqueLayer.end(), mBackgroundSprite),
        opaqueLayer.end()
    );

    auto& transparentLayer = app->mRitemLayer[(int)Application::RenderLayer::Transparent];
    for (auto* option : mOptions)
    {
        transparentLayer.erase(
            std::remove(transparentLayer.begin(), transparentLayer.end(), option),
            transparentLayer.end()
        );
    }
}
