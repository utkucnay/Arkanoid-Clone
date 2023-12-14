#ifndef __ENERGY_BALL_H__
#define __ENERGY_BALL_H__

#include "cocos2d.h"
#include "component/MoveComponent.h"
#include "component/SpriteSqueeze.h"
#include "diContainer/DIContainer.h"
#include "manager/GameManager.h"
#include "manager/TagManager.h"

namespace Arkanoid {
  class EnergyBall : public cocos2d::Node {
  public:
    bool init() override;
    CREATE_FUNC(EnergyBall);

  public:
    virtual void onEnter() override;
    virtual void update(float delta) override;
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    void inject(const DI::DIContainer &diContainer);

  private:
    virtual void onContact(cocos2d::PhysicsContact &contact, cocos2d::Node &node);
    cocos2d::Vec2 bounce(cocos2d::PhysicsContact &contact);
    cocos2d::Vec2 hitVaus(cocos2d::Node &vaus);
    void callNodeHitFunc(cocos2d::Node& node);
    void onOutArena();
    void rotateYourself(const cocos2d::Vec2& dir, float delta);

  private:
    std::shared_ptr<Components::MoveComponent> _moveComponent;
    std::shared_ptr<Components::SpriteSqueeze> _spriteSqueeze;
    cocos2d::RefPtr<cocos2d::Sequence> _hitAnimSeq;
    cocos2d::Sprite* _sprite;
    Manager::TagManager* _tagManager;
    Manager::GameManager* _gameManager;
  };
}

#endif
