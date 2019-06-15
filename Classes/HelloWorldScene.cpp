/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {


        return false;
    }

     visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);

		//update
		this->scheduleUpdate();

    }

	// create sprite 
	avator= Sprite::create("avator.png");

	//opretations of sprite
	avator->setPosition(Vec2((visibleSize.width/4)*3 + origin.x, (visibleSize.height / 4)*3 ));
	avator->setScale(0.25f);

	velocity = Vec2(-1.0f, 0.0f);
	speed = 1.0f;
	timer = 0.0f;

	movetype = 0;

	this->addChild(avator,0);

	spriteSize = 6;
	framePerSecend = 30;

	//player sprite
	player = Sprite::create("player_run.png");
	playerframesize = player->getContentSize().width / spriteSize;
	nowframe = 0;

	player->setPosition(Vec2((visibleSize.width / 3) + origin.x, (visibleSize.height / 4) + origin.y));
	//player->setPosition(Vec2(0,0));

	player->setTextureRect(Rect(0,0,playerframesize,playerframesize));
	player->getTexture()->setAliasTexParameters();
	player->setScale(8.0f);
	this->addChild(player,0);



	//lazer
	lazer = Sprite::create();
	lazer->setTextureRect(Rect(0, 0, 1, 1));
	lazer->setColor(Color3B(255, 255, 0));
	lazer->setVisible(false);
	this->addChild(lazer, 0);

	lazerScale = 0.0f;
	lazerState = 0;
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}


void HelloWorld::update(float delta)
{
	timer += delta;
	nowframe++;

	Vec2 pos = player->getPosition();
	pos += velocity*speed;
	
	////bound
	//if (pos.x > visibleSize.width||pos.x<0)
	//{
	//	velocity.x = -velocity.x;
	//}
	//if (pos.y > visibleSize.height || pos.y < 0)
	//{
	//	velocity.y = -velocity.y;
	//}
	////alpha
	//if (timer < 5.0f)
	//{
	//	int opa = avator->getOpacity();
	//	opa--;
	//	avator->setOpacity(opa);
	//}
	//round
	if (timer > 2.0f)
	{
		timer = 0;
		movetype++;
	}
	if (movetype > 3)
	{
		movetype = 0;
	}

	switch (movetype)
	{
	case 0:
		lazerState = 0;
		lazerScale = 0.0f;
		velocity = Vec2(1.5f, 0.0f);
		player->setFlippedX(false);
		player->setTextureRect(Rect(nowframe % 60 / 10 * playerframesize, 0, playerframesize, playerframesize));
		lazer->setVisible(false);
		break;
	case 1:

		if (lazerState == 0)
		{
			lazerScale += 4.0f;
		}
		else if(lazerState==1)
		{
			lazerScale -= 1.0f;
		}
		if (lazerScale > 60.0f)
		{
			lazerState = 1;
		}
		if(lazerScale<0)
		{
			lazerScale = 0;
		}
		lazer->setVisible(true);
		lazer->setAnchorPoint(Vec2(0.0f, 0.5f));
		velocity = Vec2(0.0f, 0.0f);
		lazer->setPosition(pos + Vec2(32.0f,0.0f));
		lazer->setScaleX(500.0f);
		lazer->setScaleY(lazerScale);
		break;
	case 2:
		lazerState = 0;
		lazerScale = 0.0f;
		velocity = Vec2(-1.5f, 0.0f);
		player->setFlippedX(true);
		player->setTextureRect(Rect(nowframe % 60 / 10 * playerframesize, 0, playerframesize, playerframesize));
		lazer->setVisible(false);
		break;
	case 3:
		if (lazerState == 0)
		{
			lazerScale += 10.0f;
		}
		else if (lazerState == 1)
		{
			lazerScale -= 1.0f;
		}
		if (lazerScale > 60.0f)
		{
			lazerState = 1;
		}
		if (lazerScale<0)
		{
			lazerScale = 0;
		}

		lazer->setVisible(true);
		lazer->setAnchorPoint(Vec2(1.0f, 0.5f));
		velocity = Vec2(0.0f, 0.0f);
		lazer->setPosition(pos - Vec2(32.0f, 0.0f));
		lazer->setScaleX(500.0f);
		lazer->setScaleY(lazerScale);
		break;
	}
	//avator->setPosition(pos);



	player->setPosition(pos);
	//player->setColor(Color3B(255-((timer/3.0f)*255) , 0, (timer/3.0f)*255));

	

}
