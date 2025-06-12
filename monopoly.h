#ifndef MONOPOLY__
#define MONOPOLY__

#include <iostream>
#include <string>

enum class Status { Normal, InJail, Bankruptcy };

class Player {
public:
    Player(const std::string &name = "nameless") : name_(name), id_(next_id_++) { ++TotalNum_; }
    ~Player() { --TotalNum_; };

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    int getLocation() const { return location_; }
    int getMoney() const { return money_; }
    int getUM() const { return num_units_; }
    int getCUM() const { return num_cunits_; }
    Status getStatus() const { return status_; }
    
    void updateLocation(int location) { location_ = location; }
    void updateMoney(int difference) { money_ += difference; }
    void updateUM(bool isComposite = false) {
        ++num_units_;
        if (isComposite) ++num_cunits_;
    }
    void updateStatus(Status newStat) { status_ = newStat; }
    void bankruptcy() { num_units_ = 0; num_cunits_ = 0; status_ = Status::Bankruptcy; }

    static int TotalNumPlayers() { return TotalNum_; }
private:
    static int next_id_;
    static int TotalNum_;
    int id_;
    std::string name_;
    int location_ = 0;
    int money_ = 30000;
    int num_units_ = 0;
    int num_cunits_ = 0;
    Status status_ = Status::Normal; // Normal, InJail, Bankruptcy
    
};

int Player::next_id_ = 1;
int Player::TotalNum_ = 0;

class worldPlayer {


};

#endif