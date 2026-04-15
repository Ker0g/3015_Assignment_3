#define NOMINMAX
#include "World.h"
#include "Application.h"
#include "SceneNode.h"

World::World(Application* app)
	: mSceneGraph(new SceneNode(app))
	, mApp(app)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-3.f, 3.f, -5.f, 5.f) // Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

void World::update(const GameTimer& gt)
{
	mPlayerAircraft->setVelocity(0.0f, 0.0f, 0.0f);

	Command enemyMover;
	enemyMover.category = Category::EnemyAircraft;
	enemyMover.action = [this](SceneNode& node, const GameTimer& gt)
		{
			Aircraft& enemy = static_cast<Aircraft&>(node);
			float xPos = enemy.getWorldPosition().x;
			float xVel = enemy.getVelocity().x;

			if (xVel == 0.0f)
				enemy.setVelocity(1.0f, 0.0f, 0.0f);

			if (xPos < mWorldBounds.x && enemy.getVelocity().x < 0)
				enemy.setVelocity(1.0f, 0.0f, 0.0f);
			else if (xPos > mWorldBounds.y && enemy.getVelocity().x > 0)
				enemy.setVelocity(-1.0f, 0.0f, 0.0f);

			float time = gt.TotalTime();
			float sine = 1 * sin(2 * time);


			enemy.setVelocity(enemy.getVelocity().x, sine, 0.0f);
		};

	mCommandQueue.push(enemyMover);

	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);

	playerVelocityChange();
	mSceneGraph->update(gt);
	playerPositionChange();

	if (mBackground)
	{
		float z = mBackground->getWorldPosition().z;
		if (z < -200.0f)
			mBackground->move(0, 0, 20.0f);
	}
}

void World::draw()
{
	mSceneGraph->draw();
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::buildScene()
{
	for (auto& layer : mApp->mRitemLayer)
		layer.clear();

	mApp->mAllRitems.clear();

	auto skyRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
	skyRitem->TexTransform = MathHelper::Identity4x4();
	skyRitem->ObjCBIndex = 0;
	skyRitem->Mat = mApp->mMaterials["sky"].get();
	skyRitem->Geo = mApp->mGeometries["shapeGeo"].get();
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	mApp->mRitemLayer[(int)Application::RenderLayer::Sky].push_back(skyRitem.get());
	mApp->mAllRitems.push_back(std::move(skyRitem));

	auto boxRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&boxRitem->World,
		XMMatrixScaling(10.0f, 0.5f, 20.0f) * XMMatrixTranslation(0.0f, -4.5f, 0.0f));
	XMStoreFloat4x4(&boxRitem->TexTransform, XMMatrixScaling(1.0f, 0.5f, 1.0f));
	boxRitem->ObjCBIndex = 1;
	boxRitem->Mat = mApp->mMaterials["tile0"].get();
	boxRitem->Geo = mApp->mGeometries["shapeGeo"].get();
	boxRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;
	mApp->mRitemLayer[(int)Application::RenderLayer::Opaque].push_back(boxRitem.get());
	mApp->mAllRitems.push_back(std::move(boxRitem));

	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mApp));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, -1.0);
	mPlayerAircraft->setScale(1.0f, 0.05f, 0.8f);
	mPlayerAircraft->setVelocity(mScrollSpeed, 0.0, 0.0);
	mSceneGraph->attachChild(std::move(player));

	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mApp));
	auto raptor = enemy1.get();
	raptor->setPosition(2.0, 0.1, 1.0);
	raptor->setScale(1.0f, 0.05f, 0.8f);
	raptor->setWorldRotation(0.0, 180.0f, 0.0);
	mSceneGraph->attachChild(std::move(enemy1));

	/*std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mApp));
	auto raptor2 = enemy2.get();
	raptor2->setPosition(-2.0, 0.1, 2.0);
	raptor2->setScale(1.0f, 0.05f, 0.8f);
	raptor2->setWorldRotation(0.0, 180.0f, 0.0);
	mSceneGraph->attachChild(std::move(enemy2));*/

	//std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	//mBackground = backgroundSprite.get();
	//mBackground->setPosition(0, 0, 0.0);
	//mBackground->setScale(10.0f, 10.0f, 10.0f);
	//mBackground->setWorldRotation(90.0, 0.0f, 0.0);
	//mBackground->setVelocity(0, 0, 0);
	//mSceneGraph->attachChild(std::move(backgroundSprite));

	mSceneGraph->build();
}

void World::playerPositionChange()
{
	XMFLOAT3 position = mPlayerAircraft->getWorldPosition();
	position.x = std::max(position.x, mWorldBounds.x);
	position.x = std::min(position.x, mWorldBounds.y);
	position.z = std::max(position.z, mWorldBounds.z);
	position.z = std::min(position.z, mWorldBounds.w);
	mPlayerAircraft->setPosition(position.x, position.y, position.z);
}

void World::playerVelocityChange()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0.f && velocity.z != 0.f)
		mPlayerAircraft->setVelocity(
			velocity.x / std::sqrt(2.f),
			velocity.y / std::sqrt(2.f),
			velocity.z / std::sqrt(2.f));
}