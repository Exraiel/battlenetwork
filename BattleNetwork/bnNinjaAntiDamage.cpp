#include "bnNinjaAntiDamage.h"
#include "bnDefenseAntiDamage.h"
#include "bnNinjaStar.h"
#include "bnHideTimer.h"
#include "bnAudioResourceManager.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnCharacter.h"
#include "bnSpell.h"

NinjaAntiDamage::NinjaAntiDamage(Entity* owner) : Component(owner) {
  DefenseAntiDamage::Callback onHit = [this](Spell* in, Character* owner) {
    Entity* user = in->GetHitboxProperties().aggressor;
    Battle::Tile* tile = nullptr;

    if (user) {
      tile = user->GetTile();
      owner->RegisterComponent(new HideTimer(owner, 1.0));
    }

    if (tile) {
      owner->GetField()->AddEntity(*new NinjaStar(owner->GetField(), owner->GetTeam(), 0.2f), tile->GetX(), tile->GetY());
    }

    owner->RemoveDefenseRule(defense);
    owner->FreeComponentByID(this->GetID());
    delete this;
  };

  this->defense = new DefenseAntiDamage(onHit);
  this->GetOwnerAs<Character>()->AddDefenseRule(defense);
}

NinjaAntiDamage::~NinjaAntiDamage() {
  delete defense;
}

void NinjaAntiDamage::Update(float _elapsed) {

}

void NinjaAntiDamage::Inject(BattleScene&) {
  // Does not effect battle scene
}