#include "TitleState.h"
#include "StateStack.h"
#include "Application.h"

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context)
    , mBackgroundSprite(nullptr)
    , mText(nullptr)
    , mShowText(true)
    , mTextEffectTime(0.f)
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

    auto textRitem = std::make_unique<RenderItem>();
    mText = textRitem.get();
    XMMATRIX textScale = XMMatrixScaling(2.0f, 0.3f, 1.f);
    XMMATRIX textRotate = XMMatrixRotationX(XM_PIDIV2);
    XMMATRIX textTranslate = XMMatrixTranslation(-1.f, 2.1f, 0.f);
    XMStoreFloat4x4(&mText->World, textScale * textRotate * textTranslate);
    XMStoreFloat4x4(&mText->TexTransform, XMMatrixScaling(1.f, 1.f, 1.f));
    mText->ObjCBIndex = (UINT)app->getRenderItems().size();
    mText->Mat = app->getMaterials()["PressAnyKey"].get();
    mText->Geo = app->getGeometries()["shapeGeo"].get();
    mText->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mText->IndexCount = mText->Geo->DrawArgs["quad"].IndexCount;
    mText->StartIndexLocation = mText->Geo->DrawArgs["quad"].StartIndexLocation;
    mText->BaseVertexLocation = mText->Geo->DrawArgs["quad"].BaseVertexLocation;
    app->mRitemLayer[(int)Application::RenderLayer::Transparent].push_back(textRitem.get());
    app->getRenderItems().push_back(std::move(textRitem));
}

void TitleState::draw()
{
}

bool TitleState::update(const GameTimer& gt)
{
    mTextEffectTime += gt.DeltaTime();

    if (mTextEffectTime >= 0.5f)
    {
        mShowText = !mShowText;
        mTextEffectTime = 0.f;

        Application* app = getContext().app;
        auto& layer = app->mRitemLayer[(int)Application::RenderLayer::Transparent];

        if (mShowText)
        {
            auto it = std::find(layer.begin(), layer.end(), mText);
            if (it == layer.end())
                layer.push_back(mText);
        }
        else
        {
            layer.erase(
                std::remove(layer.begin(), layer.end(), mText),
                layer.end()
            );
        }
    }

    return true;
}

bool TitleState::handleInput()
{
    for (int key = 0x08; key < 0xFF; ++key)
    {
        if (key == VK_RETURN || key == VK_UP || key == VK_DOWN ||
            key == VK_ESCAPE || key == VK_BACK)
            continue;

        if (GetAsyncKeyState(key) & 0x8000)
        {
            requestStackPop();
            requestStackPush(States::Menu);
            return false;
        }
    }

    return false;
}

TitleState::~TitleState()
{
    Application* app = getContext().app;

    auto& opaqueLayer = app->mRitemLayer[(int)Application::RenderLayer::Opaque];
    opaqueLayer.erase(
        std::remove(opaqueLayer.begin(), opaqueLayer.end(), mBackgroundSprite),
        opaqueLayer.end()
    );

    auto& transparentLayer = app->mRitemLayer[(int)Application::RenderLayer::Transparent];
    transparentLayer.erase(
        std::remove(transparentLayer.begin(), transparentLayer.end(), mText),
        transparentLayer.end()
    );
}