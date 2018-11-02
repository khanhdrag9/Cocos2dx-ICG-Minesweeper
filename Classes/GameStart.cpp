#include "GameStart.h"
#include "GamePlay.h"

USING_NS_CC;
GameStart::GameStart()
{
	
}

GameStart::~GameStart()
{
	
}

Scene* GameStart::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = GameStart::create();
	scene->addChild(layer);
	return scene;
}


bool GameStart::init()
{
	if (!Layer::init())
		return false;

	auto screenSize = Director::getInstance()->getVisibleSize();

	auto title = Label::createWithTTF(TITLE, FONT_PATH, GRID_SIZE );
	title->setPosition(screenSize.width * 0.5f, screenSize.height * 0.7f);
	title->setColor(Color3B::WHITE);
	this->addChild(title);

	auto play = Label::createWithTTF(PLAY_LABEL, FONT_PATH, GRID_SIZE * 0.7);
	play->setPosition(screenSize.width * 0.5f, screenSize.height * 0.4f);
	play->setColor(Color3B::YELLOW);
	this->addChild(play);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*, Event*) {return true; };
	listener->onTouchEnded = CC_CALLBACK_2(GameStart::onTouchRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameStart::onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto scene = GamePlay::createScene();
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(1.0, scene));
}