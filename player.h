#ifndef PLAYER__
#define PLAYER__

#include <iostream>
#include <vector>
#include <string>

enum class Status { Normal, InJail, Bankruptcy };

// ===== Player =====
class Player {
public:
    Player(const std::string &name = "nameless") : id_(next_id_++), name_(name) {}
    ~Player() {}

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    int getLocation() const { return location_; }
    int getMoney() const { return money_; }
    int getUM() const { return num_units_; }
    int getCUM() const { return num_cunits_; }
    Status getStatus() const { return status_; }
    
    void updateLocation(int location) { location_ = location; }
    void updateUM(bool isComposite = false) {
        ++num_units_;
        if (isComposite) ++num_cunits_;
    }
    void updateStatus(Status newStat) { status_ = newStat; }
    void bankruptcy() { num_units_ = 0; num_cunits_ = 0; status_ = Status::Bankruptcy; }
    void receive(int money) { money_ += money; }
    void pay(Player* player, int payment) { money_ -= payment; player->receive(payment); }
    void pay(int payment) { money_ -= payment;}
    

private:
    static int next_id_;
    int id_ = 0;
    std::string name_;
    int location_ = 0;
    int money_ = 30000;
    int num_units_ = 0;
    int num_cunits_ = 0;
    Status status_ = Status::Normal; // Normal, InJail, Bankruptcy
    
};

int Player::next_id_ = 1;

// ===== World Player =====
class worldPlayer {
public:
  worldPlayer(){
    int num_player = 0;
    std::cin >> num_player;
    for(int i = 0; i < num_player; ++i) {
        std::string name;
        std::cin >> name;
        players_.push_back(new Player(name));
    }
  }
  ~worldPlayer(){
    for (auto player : players_) {
      delete player;
    }
  }

  Player* playerNow(int index) const {
    return (index >= 0 && index < players_.size()) ? players_[index] : nullptr;
  }
  int getPlayerCount() const {
    return players_.size(); 
  }
private:
  std::vector<Player*> players_;
};

#endif