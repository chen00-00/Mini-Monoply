#include "player.h"
#include "map.h" // Include map.h to get full definition of MapUnit


// ================== Player ==================
Player::Player(int id, const std::string& name) : id_(id), name_(name) {}

int Player::getId() const { return id_; }
const std::string& Player::getName() const { return name_; }
int Player::getMoney() const { return money_; }
int Player::getLocation() const { return location_; }
PlayerStatus Player::getStatus() const { return status_; }
int Player::getUnitCount() const { return owned_units_.size(); }

// Gets the count of only 'Collectable' type units, needed for fine calculation
int Player::getNumCollectableUnits() const {
    int count = 0;
    for (const auto& unit : owned_units_) {
        if (unit->type() == "C") {
            count++;
        }
    }
    return count;
}

int Player::pay(int amount) {
    int payment = amount;
    if (money_ < amount) {
        payment = money_;
    }
    money_ -= amount;
    return payment;
}

void Player::receive(int amount) {
    money_ += amount;
}

void Player::moveTo(int new_location, WorldMap* map) {
    // Remove player from the old location's list
    if(map->getUnit(location_)) {
        map->getUnit(location_)->removePlayerHere(this);
    }
    location_ = new_location;
    // Add player to the new location's list
    if(map->getUnit(location_)) {
        map->getUnit(location_)->addPlayerHere(this);
    }
}

void Player::addUnit(MapUnit* unit) {
    owned_units_.push_back(unit);
    unit->setHost(this);
}

// When a player goes bankrupt, release all their properties
void Player::releaseAllUnits() {
    for (auto& unit : owned_units_) {
        unit->reset(); // Reset unit to default state (no owner, level 1, etc.)
    }
    owned_units_.clear();
}

void Player::setToJail() {
    status_ = PlayerStatus::InJail;
}

void Player::releaseFromJail() {
    status_ = PlayerStatus::Normal;
}

void Player::declareBankruptcy() {
    status_ = PlayerStatus::Bankrupt;
    Player::releaseAllUnits();
}

// ================== World Player ==================
WorldPlayer::WorldPlayer(int num_player, std::vector<std::string>& Names){
    for(int i = 0; i < num_player; ++i) {
        players_.push_back(new Player(i, Names[i]));
    }
}
WorldPlayer::~WorldPlayer(){
    for (auto player : players_) {
        delete player;
    }
}

Player* WorldPlayer::playerNow(int index) const {
    return (index >= 0 && index < players_.size()) ? players_[index] : nullptr;
}

int WorldPlayer::getPlayerCount() const {
    return players_.size();
}
