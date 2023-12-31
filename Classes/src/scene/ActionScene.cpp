#include "scene/ActionScene.h"
#include "GameInstall.h"
#include "actor/Brick.h"
#include "actor/Vaus.h"
#include "actor/EnergyBall.h"
#include "actor/Column.h"
#include "component/HealthComponent.h"
#include "label/ScoreLabel.h"
#include "manager/LevelManager.h"
#include "prefab/BrickPrefab.h"
#include "resource/Resource.h"
#include "config/sceneConfig/ActionSceneConfig.h"
#include <string>

bool
Arkanoid::ActionScene::init() {

  if(!Scene::init()) {
    return false;
  }
  if(!initWithPhysics()) {
    cocos2d::log("Not init Physics!");
    return false;
  }

  _gameManager = gameDIContainer.getSingle<Manager::GameManager>();

  auto config = getActionSceneConfig();

  Vaus* vaus = Vaus::create();
  vaus->inject(config->vaus.diContainer);
  vaus->inject(gameDIContainer);

  setNodeConfig(vaus, config->vaus.nodeConfig);
  setPhysicConfig(vaus, config->vaus.physicConfig);

  auto* energyBall = Arkanoid::EnergyBall::create();
  energyBall->inject(config->energyBall.diContainer);
  energyBall->inject(gameDIContainer);
  setNodeConfig(energyBall, config->energyBall.nodeConfig);
  setPhysicConfig(energyBall, config->energyBall.physicConfig);

  Column* columnLeft = Column::create();
  columnLeft->inject(config->columnLeft.diContainer);
  columnLeft->inject(gameDIContainer);

  columnLeft->setSpriteAndAnim(
      *Resource::createColumnLeft(),
      Resource::createColumnLeftHitAnim());

  setNodeConfig(columnLeft, config->columnLeft.nodeConfig);
  setPhysicConfig(columnLeft, config->columnLeft.physicConfig);

  cocos2d::Node* sColumnLeft = cocos2d::Node::create();
  setPhysicConfig(sColumnLeft, config->columnLeft.physicConfig);
  sColumnLeft->setPosition(columnLeft->getPosition());
  sColumnLeft->setPositionX(
      columnLeft->getPositionX() -
      config->columnLeft.physicConfig.sizeBox.width);
  setPhysicConfig(sColumnLeft, config->columnLeft.physicConfig);

  Column* columnUp = Column::create();
  columnUp->inject(config->columnUp.diContainer);
  columnUp->inject(gameDIContainer);

  columnUp->setSpriteAndAnim(
      *Resource::createColumnUp(),
      Resource::createColumnUpHitAnim());

  setNodeConfig(columnUp, config->columnUp.nodeConfig);
  setPhysicConfig(columnUp, config->columnUp.physicConfig);

  Column* columnRight = Column::create();
  columnRight->inject(config->columnRight.diContainer);
  columnRight->inject(gameDIContainer);
  columnRight->setSpriteAndAnim(
      *Resource::createColumnRight(),
      Resource::createColumnRightHitAnim());

  setNodeConfig(columnRight, config->columnRight.nodeConfig);
  setPhysicConfig(columnRight, config->columnRight.physicConfig);

  cocos2d::Node* sColumnRight = cocos2d::Node::create();
  setPhysicConfig(sColumnRight, config->columnLeft.physicConfig);
  sColumnRight->setPosition(columnLeft->getPosition());
  sColumnRight->setPositionX(
      columnRight->getPositionX() +
      config->columnRight.physicConfig.sizeBox.width);
  setPhysicConfig(sColumnRight, config->columnLeft.physicConfig);

  auto endArea = cocos2d::Node::create();
  setNodeConfig(endArea, config->endArea.nodeConfig);
  setPhysicConfig(endArea, config->endArea.physicConfig);

  cocos2d::Label* highScoreLabel = cocos2d::Label::create();
  setLabelConfig(highScoreLabel, config->highScoreLabel);

  cocos2d::Label* levelLabel = cocos2d::Label::create();
  setLabelConfig(levelLabel, config->levelLabel);

  cocos2d::Label* scoreLabel = cocos2d::Label::create();
  setLabelConfig(scoreLabel, config->scoreLabel);

  auto score = Arkanoid::Label::ScoreLabel::create();
  score->inject(gameDIContainer);
  setNodeConfig(score, config->score.nodeConfig);

  _bricks = cocos2d::Node::create();

  this->addChild(vaus);
  this->addChild(energyBall);
  this->addChild(columnUp);
  this->addChild(columnLeft);
  this->addChild(sColumnLeft);
  this->addChild(columnRight);
  this->addChild(sColumnRight);
  this->addChild(highScoreLabel);
  this->addChild(levelLabel);
  this->addChild(scoreLabel);
  this->addChild(score);
  this->addChild(endArea);
  this->addChild(_bricks);

  syncLevel();

  this->scheduleUpdate();

  return true;
}

void
Arkanoid::ActionScene::update(float delta) {
  Scene::update(delta);
  if(_bricks->getChildren().size() <= 1) {
    _gameManager->requestNextLevel();
  }
}

void
Arkanoid::ActionScene::onExit() {
  auto levelManager = gameDIContainer.getSingle<Manager::LevelManager>();
  levelManager->setSnapshot(_bricks);
}

void
Arkanoid::ActionScene::syncLevel() {
  auto levelManager = gameDIContainer.getSingle<Manager::LevelManager>();

  auto levelLabel = cocos2d::Label::createWithTTF(
      std::to_string(levelManager->getLevel() + 1),
      Resource::getTTFFontFileName(),
      10);

  levelLabel->setPosition(cocos2d::Vec2(66, 338));
  levelLabel->setAnchorPoint(cocos2d::Vec2(0, 1));
  this->addChild(levelLabel);

  levelManager->createLevel(_bricks);
}

cocos2d::Scene*
Arkanoid::ActionScene::createScene() {
  return ActionScene::create();
}
