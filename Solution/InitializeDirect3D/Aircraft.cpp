#include "Aircraft.h"
#include "Application.h"
#include "Category.h"

Aircraft::Aircraft(Type type, Application* app) : Entity(app)
, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "Eagle";
		break;
	case (Raptor):
		mSprite = "Raptor";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case Type::Eagle:
		return Category::PlayerAircraft;

	default:
		return Category::EnemyAircraft;
	}
}

void Aircraft::drawCurrent() const
{
}

void Aircraft::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = app->getRenderItems().size();
	renderer->Mat = app->getMaterials()[mSprite].get();
	renderer->Geo = app->getGeometries()["shapeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
	app->mRitemLayer[(int)Application::RenderLayer::Opaque].push_back(render.get());
	app->getRenderItems().push_back(std::move(render));
}
