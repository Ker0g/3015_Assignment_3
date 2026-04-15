#pragma once
#include "Entity.h"

class SpriteNode :
	public Entity
{
public:
	SpriteNode(Application* app);

private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();
};
