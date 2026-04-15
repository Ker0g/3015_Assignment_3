#pragma once
#include "../../Common/MathHelper.h"
#include "SceneNode.h"
#include "Aircraft.h"
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "Command.h"

class Game;
class World
{
public:
	explicit							World(Application* window);
	void								update(const GameTimer& gt);
	void								draw();

	void								buildScene();

	CommandQueue& getCommandQueue();
	CommandQueue mCommandQueue;

	void playerPositionChange();
	void playerVelocityChange();

private:

	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Application* mApp;

	SceneNode* mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;

	XMFLOAT4							mWorldBounds;
	XMFLOAT2		    				mSpawnPosition;
	float								mScrollSpeed;
	Aircraft* mPlayerAircraft;
	SpriteNode* mBackground;
	Aircraft* mEnemy; 
};
