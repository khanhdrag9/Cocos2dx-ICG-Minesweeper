#pragma once
#include "cocos2d.h"
#include "Defines.h"

class GameStart : public cocos2d::Layer
{
public:
	GameStart();
	virtual ~GameStart();

	static cocos2d::Scene* createScene();

	bool init() override;

	CREATE_FUNC(GameStart);

	void onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event);
};
