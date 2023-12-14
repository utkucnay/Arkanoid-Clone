#include "manager/GameManager.h"
#include "diContainer/DIContainer.h"
#include "manager/SceneManager.h"
#include "scene/ActionScene.h"
#include "scene/TitleScene.h"

void
Arkanoid::Manager::GameManager::inject(
    const DI::DIContainer& diContainer )
{
  _sceneManager = diContainer.getSingle<SceneManager>();
  _levelManager = diContainer.getSingle<LevelManager>();
}

void
Arkanoid::Manager::GameManager::startSession() {
  auto* delay = cocos2d::DelayTime::create(2);

  auto* changeScene = cocos2d::CallFunc::create([=] () {
          _sceneManager->changeScene<ActionScene>();
        }
      );

  auto* seq = cocos2d::Sequence::create(delay, changeScene, NULL);
  cocos2d::Director::getInstance()->getRunningScene()->runAction(seq);
}

void
Arkanoid::Manager::GameManager::endSession() {

}

void
Arkanoid::Manager::GameManager::winLevel() {
  _levelManager->nextLevel();
}

void
Arkanoid::Manager::GameManager::onBallOutSpace() {
  if(nullptr == _vaus) {
    cocos2d::log("Vaus dont find");
    return;
  }
  _vaus->startDestroyVaus();
}

void
Arkanoid::Manager::GameManager::endDestroyVaus(bool isDeath) {
  if(isDeath) {
    _levelManager->resetLevel();
    _sceneManager->changeScene<TitleScene>();
  } else {
    _sceneManager->changeScene<TitleScene>();
  }
}