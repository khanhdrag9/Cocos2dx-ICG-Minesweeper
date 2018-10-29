#include "GamePlay.h"

USING_NS_CC;
GamePlay::GamePlay():
	_level(NORMAL)
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

	_screenSize = Director::getInstance()->getVisibleSize();

	createGrid();
	createAnimation();

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

void GamePlay::onToucMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	for (auto x : _grids)
	{
		if (x.CheckContainPoint(touch->getLocation()))
		{
			_gridClicked->setVisible(true);
			_gridClicked->setPosition(x._point + _gridClicked->getContentSize() * 0.5);
		}
	}
}

void GamePlay::onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_gridClicked->setVisible(false);
}