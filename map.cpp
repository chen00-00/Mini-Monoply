#include "map.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib> 
#include <ctime>

 

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
    player->pay(fine);
    host_->receive(fine);
  }
}

std::string UpgradableUnit::type() const{ 
  return "U"; 
}

void UpgradableUnit::reset() {
  level_ = 1;
  host_ = nullptr;
}

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
    player->pay(total_fine);
    host_->receive(total_fine);
  }
}

std::string RandomCostUnit::type() const { 
  return "R"; 
}

void RandomCostUnit::reset() {
  host_ = nullptr;
}

// ================== World Map ====================
WorldMap::WorldMap() {
  std::ifstream in("map.dat");
  if (!in) {
      std::cerr << "Failed to open map.dat\n";
      return;
  }

  std::string line;
  int id = 1; // Start IDs from 1

  while (std::getline(in, line)) {
    std::cout<< "Processing line: " << line << std::endl;
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
      else if (type == 'C') {
      int price, fine;
      iss >> price >> fine;
      units_.push_back(new CollectableUnit(id++, name, price, fine));
    } 
    else if (type == 'R') {
      int price, fine;
      iss >> price >> fine;
      units_.push_back(new RandomCostUnit(id++, name, price, fine));
    } 
    else if (type == 'J') {
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
