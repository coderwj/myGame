#include <iostream>
#include "GameScene.h"

int main(){
    GameScene * pGameScene = GameScene::getInstance();
    if(pGameScene == NULL)
    {
        std::cout << "GameScene Create error!" << std::endl;
        return 0;
    }
    if(!pGameScene->init())
    {
        std::cout << "GameScene Init error!" << std::endl;
        return 0;
    }
    std::cout << "-----------GameScene start-----------" << std::endl;
    return 0;
}