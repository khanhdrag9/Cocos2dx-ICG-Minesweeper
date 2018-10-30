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
	listener->onTouchBegan = [](Touch*, Event*) {return true; };
	listener->onTouchMoved = CC_CALLBACK_2(GamePlay::onToucMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GamePlay::createGrid()
{
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
		Sprite* p = Sprite::create("grid.png");
		p->setContentSize(Size(GRID_SIZE, GRID_SIZE));
		p->setPosition(x._point + p->getContentSize() * 0.5);
		p->setOpacity(90);
		this->addChild(p);
	}
#endif
}

void GamePlay::createAnimation()
{
	_gridClicked = Sprite::create("grid.png");
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

void GamePlay::onToucMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	for (int i = 0; i < _grids.size(); i++)
	{
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