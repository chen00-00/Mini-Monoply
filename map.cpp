#include "map.h"
#include "player.h" // Needed for onVisit implementations
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

// ===== MapUnit (base class) =====
void MapUnit::addPlayerHere(Player* p) {
  who_is_here_.push_back(p);
}

void MapUnit::removePlayerHere(Player* p) {
    who_is_here_.erase(
        std::remove(who_is_here_.begin(), who_is_here_.end(), p),
        who_is_here_.end()
    );
}

const std::vector<Player*>& MapUnit::getPlayersHere() const {
  return who_is_here_;
}

// ================== Upgradable Unit ====================
UpgradableUnit::UpgradableUnit(int id, const std::string& name, int price, int upgrade_price, const int* fines)
    : MapUnit(id, name), price_(price), upgrade_price_(upgrade_price), level_(1) {
  for (int i = 0; i < 5; ++i) {
    fines_[i] = fines[i];
  }
}

void UpgradableUnit::onVisit(Player* player) {
  if (host_ && host_ != player) {
    int fine = getFine();
    std::cout << player->getName() << ", you must pay $" << fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
    int payment = player->pay(fine);
    host_->receive(payment);
  }
}

std::string UpgradableUnit::type() const{ return "U"; }

void UpgradableUnit::reset() {
  level_ = 1;
  host_ = nullptr;
}

int UpgradableUnit::getPrice() const { return price_; }
int UpgradableUnit::getUpgradePrice() const { return upgrade_price_; }
int UpgradableUnit::getLevel() const { return level_; }

void UpgradableUnit::upgrade() {
  if (level_ < 5) ++level_;
}

int UpgradableUnit::getFine() const {
  return fines_[level_ - 1];
}

// ================== Random Cost Unit ====================
RandomCostUnit::RandomCostUnit(int id, const std::string& name, int price, int fine_per_point)
  : MapUnit(id, name), price_(price), fine_per_point_(fine_per_point) {}

void RandomCostUnit::onVisit(Player* player) {
  if (host_ && host_ != player) {
    int dice = rand() % 6 + 1;
    int total_fine = dice * fine_per_point_;
    std::cout << player->getName() << ", you must pay $" << total_fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
    int payment = player->pay(total_fine);
    host_->receive(payment);
  }
}

std::string RandomCostUnit::type() const { return "R"; }
void RandomCostUnit::reset() { host_ = nullptr; }
int RandomCostUnit::getPrice() const { return price_; }


// ================== Collectable Unit ====================
CollectableUnit::CollectableUnit(int id, const std::string& name, int price, int unit_fine)
    : MapUnit(id, name), price_(price), unit_fine_(unit_fine) {}

void CollectableUnit::onVisit(Player* player) {
    if (host_ && host_ != player) {
        int num_owned = host_->getNumCollectableUnits();
        int fine = num_owned * unit_fine_; // Fine depends on how many the owner has
        std::cout << player->getName() << ", you must pay $" << fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
        int payment = player->pay(fine);
        host_->receive(payment);
    }
}

std::string CollectableUnit::type() const { return "C"; }
void CollectableUnit::reset() { host_ = nullptr; }
int CollectableUnit::getPrice() const { return price_; }


// ================== Jail Unit ====================
JailUnit::JailUnit(int id, const std::string& name) : MapUnit(id, name) {}

void JailUnit::onVisit(Player* player) {
    std::cout << player->getName() << " is visiting the Jail. He (She) will be frozen for one round.";
    player->setToJail(); // Player is frozen for one round
}

std::string JailUnit::type() const { return "J"; }


// ================== World Map ====================
WorldMap::WorldMap() {
  std::ifstream in("map.dat");
  if (!in) {
      std::cerr << "Failed to open map.dat\n";
      return;
  }

  std::string line;
  int id = 0; // Start IDs from 0 to match vector indices

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    char type;
    std::string name;
    iss >> type >> name;

    if (type == 'U') {
      int price, upgrade_price, fines[5];
      iss >> price >> upgrade_price;
      for (int i = 0; i < 5; ++i) iss >> fines[i];
      units_.push_back(new UpgradableUnit(id++, name, price, upgrade_price, fines));
    }
    else if (type == 'C') { // Handle Collectable type
      int price, fine;
      iss >> price >> fine;
      units_.push_back(new CollectableUnit(id++, name, price, fine));
    }
    else if (type == 'R') { // Handle RandomCost type
      int price, fine_per_point;
      iss >> price >> fine_per_point;
      units_.push_back(new RandomCostUnit(id++, name, price, fine_per_point));
    }
    else if (type == 'J') { // Handle Jail type
      units_.push_back(new JailUnit(id++, name));
    }
  }
}

WorldMap::~WorldMap() {
  for (auto unit : units_) {
      delete unit;
  }
}

MapUnit* WorldMap::getUnit(int index) const {
  return (index >= 0 && index < units_.size()) ? units_[index] : nullptr;
}

int WorldMap::getUnitCount() const {
  return units_.size();
}
