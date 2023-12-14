#include "actor/EnergyBall.h"
#include "2d/CCActionInterval.h"
#include "actor/IHit.h"
#include "resource/Resource.h"

bool
Arkanoid::EnergyBall::init() {
  auto contactListener = cocos2d::EventListenerPhysicsContact::create();
  contactListener->onContactBegin =
    CC_CALLBACK_1(EnergyBall::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(
      contactListener, this);

  _sprite = Resource::createEnergyBall();
  this->addChild(_sprite);

  this->scheduleUpdate();
  return true;
}

void
Arkanoid::EnergyBall::inject(
    const DI::DIContainer& diContainer)
{
  if(diContainer.hasFactory<Components::MoveComponent>()) {
    _moveComponent = diContainer.getFactory
      <Components::MoveComponent>();

    _moveComponent->setOwner(*this);
  }
  if(diContainer.hasFactory<Components::SpriteSqueeze>()){
    _spriteSqueeze = diContainer.getFactory
      <Components::SpriteSqueeze>();

    _spriteSqueeze->setOwner(*this);
  }
  if(diContainer.hasSingle<Manager::TagManager>())
    _tagManager = diContainer.getSingle<Manager::TagManager>();
  if(diContainer.hasSingle<Manager::SceneManager>())
    _gameManager = diContainer.getSingle<Manager::GameManager>();

}

void
Arkanoid::EnergyBall::onEnter() {
  Node::onEnter();
  _spriteSqueeze->setSqueeze(cocos2d::Vec2(1.25, .75));
  _moveComponent->onEnter();
  _moveComponent->setVelocity(cocos2d::Vec2(5, 10));
  _moveComponent->setSpeed(100);

  auto fs = cocos2d::ScaleTo::create(.1f, 1.5, .5);
  auto dl = cocos2d::DelayTime::create(.2f);
  auto fs2 = cocos2d::ScaleTo::create(.1f, 1, 1);
  auto dl2 = cocos2d::DelayTime::create(.2f);
  auto fs3 = cocos2d::ScaleTo::create(.1f, .5, 1.5);
  auto dl3 = cocos2d::DelayTime::create(.2f);
  auto fs4 = cocos2d::ScaleTo::create(.1f, 1.25, .75);
  auto dl4 = cocos2d::DelayTime::create(.2f);
  auto fs5 = cocos2d::ScaleTo::create(.1f, 1, 1);
  _hitAnimSeq = cocos2d::Sequence::create(fs, dl, fs2, dl2, fs3, dl3,
        fs4, dl4, fs5, NULL);
  _hitAnimSeq->setTag(10);
  setPositionZ(4);
}

void
Arkanoid::EnergyBall::update(float delta) {
  Node::update(delta);
  _moveComponent->update(delta);

  if(this->getNumberOfRunningActionsByTag(10) == 0)
    _spriteSqueeze->setScale(_moveComponent->getSpeed() / 400);

  rotateYourself(_moveComponent->getDir(), delta);
}

bool
Arkanoid::EnergyBall::onContactBegin(
    cocos2d::PhysicsContact& contact)
{
  auto nodeA = contact.getShapeA()->getBody()->getNode();
  auto nodeB = contact.getShapeB()->getBody()->getNode();
  if(nodeA->getTag() == this->getTag())
    onContact(contact, *nodeB);
  if(nodeB->getTag() == this->getTag())
    onContact(contact, *nodeA);

  return false;
}

void
Arkanoid::EnergyBall::onContact (
    cocos2d::PhysicsContact& contact,
    cocos2d::Node& node)
{

  if(node.getTag() == _tagManager->getTag("EndArea")) {
    onOutArena();
    return;
  }

  callNodeHitFunc(node);
  runAction(_hitAnimSeq);
  cocos2d::Vec2 dir;
  if(node.getTag() == _tagManager->getTag("Vaus")) {
    dir = hitVaus(node);
  }
  else {
    dir = bounce(contact);
  }
  setPosition(getPosition() + dir.getNormalized() * 1);
  _moveComponent->setDir(dir.getNormalized());
}

cocos2d::Vec2
Arkanoid::EnergyBall::bounce(
    cocos2d::PhysicsContact& contact)
{
  cocos2d::Vec2 dir;
  auto normal = contact.getContactData()->normal;
  dir = _moveComponent->getDir();

  float dotp = dir.dot(normal);
  normal.scale(2 * dotp);
  dir.subtract(normal);
  return dir;
}

cocos2d::Vec2
Arkanoid::EnergyBall::hitVaus(cocos2d::Node& vaus) {
  cocos2d::Vec2 dir;
  dir = getPosition() - vaus.getPosition();
  dir.y = dir.y > 0 ? dir.y : dir.y * -1;
  return dir;
}

void
Arkanoid::EnergyBall::onOutArena() {
  _moveComponent->setSpeed(0);
  _gameManager->onBallOutSpace();
}

void
Arkanoid::EnergyBall::callNodeHitFunc(
    cocos2d::Node& node)
{
  if(auto iHit = dynamic_cast<IHit*>(&node)) {
      iHit->hit(*this);
  }
}

void
Arkanoid::EnergyBall::rotateYourself(
    const cocos2d::Vec2& dir,
    float delta)
{
  auto dot = 0 * dir.x + -1 * dir.y;
  auto det = 0 * dir.y - -1 * dir.x;
  float rotate = atan2(dot, det);
  rotate = rotate * (180 / M_PI);

  setRotation(rotate);
}
