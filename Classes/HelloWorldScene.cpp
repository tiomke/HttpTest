#include "HelloWorldScene.h"
#include "network/HttpClient.h"
using namespace cocos2d::network;

USING_NS_CC;
 
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));



	auto label = LabelTTF::create("request","Arial",24);
	auto requestBtn = MenuItemLabel::create(label,this,menu_selector(HelloWorld::menuRequestCallback));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem,requestBtn, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    requestBtn->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //// add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::menuRequestCallback(Ref* pSender)
{
	CCLOG("HelloWorld::menuRequestCallback enter!! ");
	//添加网络数据获取请求
	HttpRequest* request = new HttpRequest();

	request->setUrl("http://www.oschina.net/action/api/news_list");
	request->setRequestType(HttpRequest::Type::POST);

	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/json; charset=utf-8");
	request->setHeaders(headers);

	const char* postData = "catalog=2&pageIndex=1&pageSize=5";
	request->setRequestData(postData ,strlen(postData));
	request->setResponseCallback(this,httpresponse_selector(HelloWorld::onHttpResponseComplete));
	request->setTag("Post_My_Data");

	HttpClient::getInstance()->send(request);

	request->release();
}

void HelloWorld::onHttpResponseComplete(HttpClient* client, HttpResponse* response)
{
	//接收数据并显示
	CCLOG("1-------HelloWorld::onHttpResponseComplete enter!! ");

	if (!response) return;

	CCLOG("2---HelloWorld::onHttpResponseComplete enter!! ");
	//freopen("CONIN$","r",stdin);
	//freopen("CONOUT$","w",stdout);
	//freopen("CONOUT$","w",stderr);

	//获取响应的tag
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("response tag: %s is completed!\n",response->getHttpRequest()->getTag());
	}
	
	//获取状态码
	int stateCode = response->getResponseCode();
	CCLOG("stateCode: %d\n",stateCode);
	std::vector<char>* pData = response->getResponseData();//

	for (int i = 0; i < pData->size(); i++)
	{
		CCLOG("%c",(*pData)[i]);
	}
	CCLOG("\n");

}