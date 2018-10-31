#pragma once
#include "cocos2d.h"
#include "Defines.h"

typedef struct Grid {
	//SpriteFrame*
	cocos2d::Point _point;
	bool _isMine = false;
	bool _wasShow = false;
	bool _wasChecked = false;
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
	int _numberMine;
	int _numberSafeGrids;

	cocos2d::Size _screenSize;
	cocos2d::Sprite* _bg;
	cocos2d::Sprite* _gridClicked;
	int _gridClicking;

	std::vector<Grid> _grids;

	cocos2d::Label* _winLabel;
	cocos2d::Label* _loseLabel;
	cocos2d::Label* _resetGame;
	bool _isEndGame;

	void createGrid();
	void createAnimation();
	void createMine();
	void checkGrid();
	void scanPositionClick(int index);
	void showAllGrid();

	void winGame();
	void loseGame();
	void resetGame();

public:
	GamePlay();
	virtual ~GamePlay();

	static cocos2d::Scene* createScene();

	bool init() override;

	CREATE_FUNC(GamePlay);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event);
};
