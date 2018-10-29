#pragma once
#include "cocos2d.h"
#include "Defines.h"

typedef struct Grid {
	//SpriteFrame*
	cocos2d::Point _point;
	bool _isMine = false;
	bool _wasShow = false;
	Grid(int xx, int yy) {
		_point = cocos2d::Vec2(xx, yy);
	}
	bool CheckContainPoint(const cocos2d::Vec2& point)
	{
		if (point.x > _point.x && point.x < _point.x + GRID_SIZE &&
			point.y > _point.y && point.y < _point.y + GRID_SIZE)
			return true;
		return false;
	}
} Grid;

class GamePlay : public cocos2d::Layer
{
	int _level;

	cocos2d::Size _screenSize;
	cocos2d::Sprite* _bg;
	cocos2d::Sprite* _gridClicked;
	Grid* _gridClicking;

	std::vector<Grid> _grids;

	void createGrid();
	void createAnimation();

public:
	GamePlay();
	virtual ~GamePlay();

	static cocos2d::Scene* createScene();

	bool init() override;

	CREATE_FUNC(GamePlay);

	void onToucMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event);
};
