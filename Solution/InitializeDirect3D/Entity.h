#pragma once
#include "SceneNode.h"

class Entity :
	public SceneNode
{
public:
	Entity(Application* app); 
	void			setVelocity(XMFLOAT3 velocity);
	void			setVelocity(float vx, float vy, float vz);
	XMFLOAT3		getVelocity() const; 
	void			accelerate(XMFLOAT3 velocity);       
	void			accelerate(float vx, float vy, float vz);

	virtual	void		updateCurrent(const GameTimer& gt);

private:
	XMFLOAT3		mVelocity;
};

