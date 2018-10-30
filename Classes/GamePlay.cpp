#include "GamePlay.h"
#include <time.h>

USING_NS_CC;
GamePlay::GamePlay():
	_level(NORMAL),
	_gridClicking(-1)
{
	
}

GamePlay::~GamePlay()
{
	CC_SAFE_DELETE(_gridClicked);
	CC_SAFE_DELETE(_bg);
}

Scene* GamePlay::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = GamePlay::create();
	scene->addChild(layer);
	return scene;
}


bool GamePlay::init()
{
	if (!Layer::init())
		return false;
	srand(time(NULL));

	_screenSize = Director::getInstance()->getVisibleSize();

	createGrid();
	createAnimation();
	createMine();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GamePlay::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GamePlay::createGrid()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SPRITE_PLIST);

	_bg = Sprite::create(BACKGROUND, Rect(0, 0, _level * GRID_SIZE, _level * GRID_SIZE));
	_bg->setPosition(_screenSize * 0.5f);
	this->addChild(_bg);

	float sideGrid = GRID_SIZE;
	int px = _bg->getPositionX() - _bg->getContentSize().width * 0.5;
	int py = _bg->getPositionY() - _bg->getContentSize().height * 0.5;
	int maxX = px + GRID_SIZE * _level;
	int maxY = py + GRID_SIZE * _level;

	int tile = _level * _level;
	
	for (int r = py; r < maxY; r+=GRID_SIZE)
	{
		for (int c = px; c < maxX; c+=GRID_SIZE)
		{
			_grids.push_back(Grid(c, r));
		}
	}
	
#if TEST_GRID
	for (auto x : _grids)
	{
		Sprite* p = Sprite::createWithSpriteFrameName("grid.png");
		p->setContentSize(Size(GRID_SIZE, GRID_SIZE));
		p->setPosition(x._point + p->getContentSize() * 0.5);
		p->setOpacity(90);
		this->addChild(p);
	}
#endif
}

void GamePlay::createAnimation()
{
	_gridClicked = Sprite::createWithSpriteFrameName("grid.png");
	_gridClicked->setContentSize(Size(GRID_SIZE, GRID_SIZE));
	_gridClicked->setVisible(false);
	_gridClicked->retain();
	this->addChild(_gridClicked);
}

void GamePlay::createMine()
{
	_numberMine = _level * _level * 0.2f;
	_numberSafeGrids = _grids.size() - _numberMine;
	for (int i = 0; i < _numberMine; ++i)
	{
		int r = rand() % _grids.size();
		if (_grids[r]._isMine)i--;
		else _grids[r]._isMine = true;
	}

#if TEST_GRID
	auto rect = DrawNode::create();
	for(auto x : _grids)
		if (x._isMine)
		{	
			rect->drawCircle(x._point + Vec2(GRID_SIZE*0.5, GRID_SIZE*0.5), GRID_SIZE * 0.5f, 360, 360, false, Color4F::RED);
		}
	this->addChild(rect);
#endif
}

void GamePlay::checkGrid()
{
	if (_grids[_gridClicking]._isMine)
	{
		loseGame();
	}
	else if(_numberSafeGrids <= 0)
	{
		winGame();
	}
	else
	{
		//code logic
		scanPositionClick(_gridClicking);
	}
}
void GamePlay::scanPositionClick(int index)
{
	int numberMineAround = 0;

	std::vector<int> aroundGrid;
	int caseAroundGrid[8] = {
		index - _level - 1, index - _level, index - _level + 1,
		index - 1, index + 1,
		index + _level - 1, index + _level, index + _level + 1
	};

	for (int i = 0; i < 8; i++)
	{
		auto grid = caseAroundGrid[i];
		if (grid < 0)
			continue;
		if (grid >= _grids.size())
			continue;

		Vec2 subX = _grids[grid]._point - _grids[index]._point;
		subX.x *= (subX.x < 0 ? -1 : 1);
		subX.y *= (subX.y < 0 ? -1 : 1);
		if (subX.x > GRID_SIZE || subX.y > GRID_SIZE)
			continue;

		//calculate
		if (_grids[grid]._isMine)numberMineAround++;
		else aroundGrid.push_back(grid);
	}

	if (numberMineAround == 0)
	{
		_grids[index]._wasShow = true;

		Sprite* s = Sprite::createWithSpriteFrameName(GRID_SHOWN);
		s->setPosition(_grids[index]._point + Size(GRID_SIZE, GRID_SIZE) * 0.5);
		this->addChild(s);

		for (int i = 0; i < aroundGrid.size(); i++)
		{
			scanPositionClick(aroundGrid[i]);
		}
	}
	else //draw label
	{
		CCLOG("NUMBER");
		char num[2];
		sprintf(num, "%d", numberMineAround);
		Label* number = Label::createWithTTF(num, FONT_PATH, GRID_SIZE * 0.65);
		number->setPosition(_grids[index]._point + Size(GRID_SIZE, GRID_SIZE) * 0.5);
		this->addChild(number);
	}

}

void GamePlay::winGame()
{
	CCLOG("Win game");
}

void GamePlay::loseGame()
{
	CCLOG("Lose game");
}

bool GamePlay::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	onTouchMoved(touch, event);
	return true;
}

void GamePlay::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	for (int i = 0; i < _grids.size(); i++)
	{
		if(!_grids[i]._wasShow)
		if (_grids[i].CheckContainPoint(touch->getLocation()))
		{
			_gridClicking = i;
			_gridClicked->setVisible(true);
			_gridClicked->setPosition(_grids[i]._point + _gridClicked->getContentSize() * 0.5);
			return;
		}
	}
	_gridClicking = -1;
}

void GamePlay::onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_gridClicked->setVisible(false);
	if (_gridClicking != -1)
	{
		checkGrid();
	}
}