#include "SpriteNode.h"
#include "Application.h"

SpriteNode::SpriteNode(Application* app) : Entity(app)
{
}

void SpriteNode::drawCurrent() const
{
	renderer->World = getTransform();
	renderer->NumFramesDirty++;
}

void SpriteNode::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform(); 
	XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	renderer->ObjCBIndex = app->getRenderItems().size();
	renderer->Mat = app->getMaterials()["tile0"].get();
	renderer->Geo = app->getGeometries()["shapeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["sphere"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["quad"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["quad"].BaseVertexLocation;
	app->mRitemLayer[(int)Application::RenderLayer::Opaque].push_back(render.get());
	app->getRenderItems().push_back(std::move(render));
}
