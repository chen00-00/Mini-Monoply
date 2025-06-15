#include "map.h"
#include "player.h" // Needed for onVisit implementations
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>

// ===== MapUnit (base class) =====
MapUnit::MapUnit(int id, const std::string& name, int numPlayers)
: id_(id), name_(name), playersHerePtrs_(numPlayers, nullptr) {}

void MapUnit::addPlayerHere(Player* p) {
  int id = p->getId();
  if (id >= 0 && id < playersHerePtrs_.size()) {
    playersHerePtrs_[id] = p;
  }
}

void MapUnit::removePlayerHere(Player* p) {
  int id = p->getId();
  if (id >= 0 && id < playersHerePtrs_.size()) {
    playersHerePtrs_[id] = nullptr;
  }
}

const std::vector<Player*>& MapUnit::getPlayersHere() const {
  return playersHerePtrs_;
}

std::string MapUnit::getPlayersHereString() const {
  std::string s = "=";
  for (Player* ptr : playersHerePtrs_) {
      if (ptr) {
          s += std::to_string(ptr->getId());
      } else {
          s += " ";
      }
  }
  s += "=";
  return s;
}


const std::string MapUnit::display() const {
  std::ostringstream oss;
  std::ostringstream id_stream;
  id_stream << "[" << id_ << "]";
  oss << getPlayersHereString() << "  "
      << std::left << std::setw(5) << id_stream.str()
      << std::right << std::setw(10) << name_.substr(0, 10) << " ";
  return oss.str();
}


// ================== Purchasable Unit ====================
PurchasableUnit::PurchasableUnit(int id, const std::string& name, int numPlayers, int price)
        : MapUnit(id, name, numPlayers), price_(price), host_(nullptr) {}


void PurchasableUnit::tryToBuy(Player* player) {
  int price = getPrice();
  if (player->getMoney() >= price) {
    std::cout << player->getName() << ", do you want to buy " << getName() << "? (1: Yes [default] / 2: No) ...>";
    std::string buy_choice = "";
    std::getline(std::cin, buy_choice);
    if (buy_choice != "2") {
        player->pay(price);
        player->addUnit(this);
        setHost(player);
        std::cout << "You pay $" << price << " to buy " << getName();
    }
  }
}

const std::string PurchasableUnit::display() const {
  std::ostringstream oss;
  oss << MapUnit::display();
  if (!host_) {
      oss << std::setw(4) << "";
      oss << "B$";
      oss << std::setw(5) << std::right << price_;
  } else {
      std::string host_str = "{" + std::to_string(host_->getId()) + "}";
      oss << std::setw(4) << std::left << host_str;
  }
  return oss.str();
}

// ================== Upgradable Unit ====================
UpgradableUnit::UpgradableUnit(int id, const std::string& name, int numPlayers, int price, int upgrade_price, const int* fines)
  : PurchasableUnit(id, name, numPlayers, price), upgrade_price_(upgrade_price), level_(1) {
  for (int i = 0; i < 5; ++i) {
    fines_[i] = fines[i];
  }
}

void UpgradableUnit::onVisit(Player* player) {
  if (!host_) {
    tryToBuy(player);
  }
  else if (host_ != player) {
    // If the unit is owned by another player, the visiting player must pay a fine
    int fine = getFine();
    std::cout << player->getName() << ", you must pay $" << fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
    int payment = player->pay(fine);
    host_->receive(payment);
  }
  else if (host_ == player) {
    // upgrade if the owner is the same as the visiting player
    if (getLevel() < 5) {
      int upgrade_price = getUpgradePrice();
      if (player->getMoney() >= upgrade_price) {
           std::cout << player->getName() << ", do you want to upgrade " << getName() << "? (1: Yes [default] / 2: No)...>";
           std::string upgrade_choice = "";
           std::getline(std::cin, upgrade_choice);
           if(upgrade_choice != "2") {
              player->pay(upgrade_price);
              upgrade();
              std::cout << "You pay $" << upgrade_price << " to upgrade " << getName() << " to Lv." << getLevel();
           }
      }
    }
    else {
      std::cout << player->getName() << ", your " << getName() << " already reaches the highest level!";
    }
  }
}

const std::string UpgradableUnit::type() const{ return "U"; }

void UpgradableUnit::reset() {
  level_ = 1;
  setHost(nullptr);
}

const std::string UpgradableUnit::display() const {
  std::ostringstream oss;
  oss << PurchasableUnit::display();
  std::ostringstream status;
  if(host_) {
    if (level_ == 5) {
      oss << std::setw(3) << std::left << "L5";
    } else {
      oss << "U$";
      oss << std::setw(5) << std::right << upgrade_price_;
      oss << " L" << level_;

    }
  }
  return oss.str();
}

const int UpgradableUnit::getUpgradePrice() const { return upgrade_price_; }
const int UpgradableUnit::getLevel() const { return level_; }

void UpgradableUnit::upgrade() {
  if (level_ < 5) ++level_;
}

const int UpgradableUnit::getFine() const {
  return fines_[level_ - 1];
}


// ================== Random Cost Unit ====================
RandomCostUnit::RandomCostUnit(int id, const std::string& name, int numPlayers, int price, int finePerPoint)
  : PurchasableUnit(id, name, numPlayers, price), finePerPoint_(finePerPoint) {}

void RandomCostUnit::onVisit(Player* player) {
  if (!host_) {
    tryToBuy(player);
  }
  else if (host_ != player) {
    int dice = rand() % 6 + 1;
    int total_fine = dice * finePerPoint_;
    std::cout << player->getName() << ", you must pay $" << total_fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
    int payment = player->pay(total_fine);
    host_->receive(payment);
  }
}

const std::string RandomCostUnit::type() const { return "R"; }
void RandomCostUnit::reset() { setHost(nullptr); }

const std::string RandomCostUnit::display() const {
  std::ostringstream oss;
  oss << PurchasableUnit::display();
  if (host_) {
    oss << "?";
  }
  return oss.str();
}

// ================== Collectable Unit ====================
CollectableUnit::CollectableUnit(int id, const std::string& name, int numPlayers, int price, int unitFine)
    : PurchasableUnit(id, name, numPlayers, price), unitFine_(unitFine) {}

void CollectableUnit::onVisit(Player* player) {
  if (!host_) {
    tryToBuy(player);
  }
  else if (host_ != player) {
      int num_owned = host_->getNumCollectableUnits();
      int fine = num_owned * unitFine_; // Fine depends on how many the owner has
      std::cout << player->getName() << ", you must pay $" << fine << " to Player " << host_->getId() << " (" << host_->getName() << ")";
      int payment = player->pay(fine);
      host_->receive(payment);
  }
}

const std::string CollectableUnit::type() const { return "C"; }
void CollectableUnit::reset() { setHost(nullptr); }

const std::string CollectableUnit::display() const {
  std::ostringstream oss;
  oss << PurchasableUnit::display();
  if (host_) {
    oss << "x" << host_->getNumCollectableUnits();
  }
  return oss.str();
}


// ================== Jail Unit ====================
JailUnit::JailUnit(int id, const std::string& name, int numPlayers) : MapUnit(id, name, numPlayers) {}

void JailUnit::onVisit(Player* player) {
    std::cout << player->getName() << " is visiting the Jail. He (She) will be frozen for one round.";
    player->setToJail(); // Player is frozen for one round
}

const std::string JailUnit::type() const { return "J"; }


const std::string JailUnit::display() const {
  std::ostringstream oss;
  oss << MapUnit::display();
  oss << std::setw(4) << "";
  oss << std::setw(3) << std::left << "J";
  return oss.str();
}

// ================== World Map ====================
WorldMap::WorldMap(int numPlayers) {
  std::ifstream in("map.dat");
  if (!in) {
      std::cerr << "Failed to open map.dat\n";
      return;
  }

  std::string line;
  int id = 0;

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    char type;
    std::string name;
    iss >> type >> name;

    if (type == 'U') {
      int price, upgrade_price, fines[5];
      iss >> price >> upgrade_price;
      for (int i = 0; i < 5; ++i) iss >> fines[i];
      units_.push_back(new UpgradableUnit(id++, name, numPlayers, price, upgrade_price, fines));
    }
    else if (type == 'C') { // Handle Collectable type
      int price, unitFine;
      iss >> price >> unitFine;
      units_.push_back(new CollectableUnit(id++, name, numPlayers, price, unitFine));
    }
    else if (type == 'R') { // Handle RandomCost type
      int price, finePerPoint;
      iss >> price >> finePerPoint;
      units_.push_back(new RandomCostUnit(id++, name, numPlayers, price, finePerPoint));
    }
    else if (type == 'J') { // Handle Jail type
      units_.push_back(new JailUnit(id++, name, numPlayers));
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

const int WorldMap::getUnitCount() const {
  return units_.size();
}
