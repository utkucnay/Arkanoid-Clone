#ifndef __GAME_INNSTALL_H__
#define __GAME_INNSTALL_H__

#include "CCUserDefault.h"
#include "config/LevelConfig.h"
#include "diContainer/DIContainer.h"
#include "manager/GameManager.h"
#include "manager/LevelManager.h"
#include "manager/SceneManager.h"
#include "manager/ScoreManager.h"
#include "manager/TagManager.h"
#include "scene/ActionScene.h"
#include "scene/ShowLevelScene.h"
#include "scene/TitleScene.h"

inline DI::DIContainer gameDIContainer;

class GameInstall {
  public:
    GameInstall() {
      using namespace Arkanoid::Manager;

      gameDIContainer.addSingle<TagManager, TagManager>();
      gameDIContainer.addSingle<SceneManager, SceneManager>();
      gameDIContainer.addSingle<LevelManager, LevelManager>();
      gameDIContainer.addSingle<GameManager, GameManager>();
      gameDIContainer.addSingle<ScoreManager, ScoreManager>();

      {
        auto tagManager = gameDIContainer.getSingle<TagManager>();
        tagManager->setTag("Vaus");
        tagManager->setTag("Column");
        tagManager->setTag("EnergyBall");
        tagManager->setTag("EndArea");
      }

      {
        auto sceneManager = gameDIContainer.getSingle<SceneManager>();
        sceneManager->addScene<Arkanoid::TitleScene>();
        sceneManager->addScene<Arkanoid::ShowLevelScene>();
        sceneManager->addScene<Arkanoid::ActionScene>();
        sceneManager->inject(gameDIContainer);
        sceneManager->setScene();
      }

      {
        auto levelManager = gameDIContainer.getSingle<LevelManager>();
        levelManager->inject(gameDIContainer);
        levelManager->addLevel(level1);
        levelManager->addLevel(level2);
      }

      {
        gameDIContainer.getSingle<GameManager>()->inject(gameDIContainer);
      }

      {
        auto scoreManager = gameDIContainer.getSingle<ScoreManager>();
        auto userDefault = gameDIContainer.getCocosSingle<cocos2d::UserDefault>();
        scoreManager->inject(gameDIContainer);
        scoreManager->setHighScore(userDefault->getIntegerForKey("highScore"));
      }
    }
};

#endif
