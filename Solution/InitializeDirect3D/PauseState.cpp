#include "PauseState.h"
#include "StateStack.h"
#include "Application.h"
#include <algorithm>

PauseState::PauseState(StateStack& stack, Context context)
    : State(stack, context)
    , mPausedText(nullptr)
    , mInstructionText(nullptr)
    , mEscapeWasDown(true)
    , mBackspaceWasDown(false)
{
    Application* app = getContext().app;

    auto overlayRitem = std::make_unique<RenderItem>();
    mOverlay = overlayRitem.get();
    XMMATRIX oScale = XMMatrixScaling(100.0f, 100.0f, 100.0f);
    XMMATRIX oRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX oTranslate = XMMatrixTranslation(-10.f, 0.3f, 5.f);
    XMStoreFloat4x4(&mOverlay->World, oScale * oRotate * oTranslate);
    XMStoreFloat4x4(&mOverlay->TexTransform, XMMatrixScaling(1.f, 1.f, 1.f));
    mOverlay->ObjCBIndex = (UINT)app->getRenderItems().size(); 
    mOverlay->Mat = app->getMaterials()["Dim"].get();
    mOverlay->Geo = app->getGeometries()["shapeGeo"].get();
    mOverlay->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mOverlay->IndexCount = mOverlay->Geo->DrawArgs["quad"].IndexCount;
    mOverlay->StartIndexLocation = mOverlay->Geo->DrawArgs["quad"].StartIndexLocation;
    mOverlay->BaseVertexLocation = mOverlay->Geo->DrawArgs["quad"].BaseVertexLocation;
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(overlayRitem.get());
    app->getRenderItems().push_back(std::move(overlayRitem));

    auto pausedRitem = std::make_unique<RenderItem>();
    mPausedText = pausedRitem.get();
    XMMATRIX pScale = XMMatrixScaling(2.f, 0.5f, 1.f);
    XMMATRIX pRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX pTranslate = XMMatrixTranslation(-1.0f, 0.4f, 1.f);
    XMStoreFloat4x4(&mPausedText->World, pScale * pRotate * pTranslate);
    XMStoreFloat4x4(&mPausedText->TexTransform, XMMatrixScaling(0.98f, 0.98f, 1.f));
    mPausedText->ObjCBIndex = (UINT)app->getRenderItems().size();
    mPausedText->Mat = app->getMaterials()["GamePaused"].get();
    mPausedText->Geo = app->getGeometries()["shapeGeo"].get();
    mPausedText->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mPausedText->IndexCount = mPausedText->Geo->DrawArgs["quad"].IndexCount;
    mPausedText->StartIndexLocation = mPausedText->Geo->DrawArgs["quad"].StartIndexLocation;
    mPausedText->BaseVertexLocation = mPausedText->Geo->DrawArgs["quad"].BaseVertexLocation;
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(pausedRitem.get());
    app->getRenderItems().push_back(std::move(pausedRitem));

    auto instrRitem = std::make_unique<RenderItem>();
    mInstructionText = instrRitem.get();
    XMMATRIX iScale = XMMatrixScaling(5.f, 0.5f, 5.f);
    XMMATRIX iRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX iTranslate = XMMatrixTranslation(-2.0f, 0.4f, 0.0f);
    XMStoreFloat4x4(&mInstructionText->World, iScale * iRotate * iTranslate);
    XMStoreFloat4x4(&mInstructionText->TexTransform, XMMatrixScaling(0.98f, 0.98f, 1.f));
    mInstructionText->ObjCBIndex = (UINT)app->getRenderItems().size();
    mInstructionText->Mat = app->getMaterials()["PauseInstruction"].get();
    mInstructionText->Geo = app->getGeometries()["shapeGeo"].get();
    mInstructionText->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mInstructionText->IndexCount = mInstructionText->Geo->DrawArgs["quad"].IndexCount;
    mInstructionText->StartIndexLocation = mInstructionText->Geo->DrawArgs["quad"].StartIndexLocation;
    mInstructionText->BaseVertexLocation = mInstructionText->Geo->DrawArgs["quad"].BaseVertexLocation;
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(instrRitem.get());
    app->getRenderItems().push_back(std::move(instrRitem));
}

void PauseState::draw()
{
}


bool PauseState::update(const GameTimer& gt)
{
    return false;
}

bool PauseState::handleInput()
{
    bool escDown = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
    if (escDown && !mEscapeWasDown)
    {
        mEscapeWasDown = true;
        requestStackPop(); 
    }
    else if (!escDown)
        mEscapeWasDown = false;

    bool bsDown = GetAsyncKeyState(VK_BACK) & 0x8000;
    if (bsDown && !mBackspaceWasDown)
    {
        mBackspaceWasDown = true;
        requestStateClear();          
        requestStackPush(States::Menu); 
    }
    else if (!bsDown)
        mBackspaceWasDown = false;

    return false;
}

PauseState::~PauseState()
{
    Application* app = getContext().app;
    auto& layer = app->mRitemLayer[(int)Application::RenderLayer::Transparent];
    layer.erase(std::remove(layer.begin(), layer.end(), mOverlay), layer.end());
    layer.erase(std::remove(layer.begin(), layer.end(), mPausedText), layer.end());
    layer.erase(std::remove(layer.begin(), layer.end(), mInstructionText), layer.end());
}