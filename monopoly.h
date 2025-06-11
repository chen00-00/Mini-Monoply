#ifndef MONOPOLY__
#define MONOPOLY__

#include <iostream>
#include <string>

class Player {
public:
    Player(const std::string &name = "nameless") { ++id_; }
    ~Player();
    int getId() const { return id_; }
    std::string getName() const { return name_; }
    int getLocation() const { return location_; }
    int getMoney() const { return money_; }
    int getUM() const { return num_units_; }
    int getCUM() const { return num_cunits_; }
    void updateLocation(int location) { location_ = location; }
    void updateMoney(int difference) { money_ += difference; }
    void updateUM(char type) { ++num_units_; if(type == 'C') { ++num_cunits_; } }
    void bankruptcy() { num_units_ = 0; num_cunits_ = 0; status_ = "bankruptcy"; }
private:
    static int id_;
    std::string name_;
    int location_ = 0;
    int money_ = 30000;
    int num_units_ = 0;
    int num_cunits_ = 0;
    std::string status_ = "normal"; // normal, inJail, bankruptcy 
}

int Player::id_ = 0;

class worldPlayer {


}

class CollectableUnit : public MapUnit {
public:
    CollectableUnit(int id = -1, const std::string &name = "unknown", int price = 0, int fine = 0):id_(id), name_(name), price_(price), fine_(fine) {}
    ~CollectableUnit();
    virtual ~MapUnit() = default;
    // int getId() const { return id_; }
    // std::string getName() const  { return name_; }
    // int getStay() const { return who_is_here_; }
    int getPrice() const  { return price_; }
    int getFine() const  { return fine_ * num_cunits_; }
    int getHost() const { return host_; }
    int getCUM() const { return num_cunits_; }
    void updateHost(const Player &p) { host_ = p.id_; num_cunits_ = p.num_cunits_; }
    void reset() { host_ = -1; num_cunits_ = 0; }
private:
    // int id_ = 0;
    // std::string name_;
    // int who_is_here_ = -1;
    int price_ = 0;
    int fine_ = 0;
    int host_ = -1;         // default: -1(nobody)
    int num_cunits_ = 0;    // number of the CollectableUnit that the host has
}

class JailUnit : public MapUnit {
public:
    JailUnit(int id = -1, const std::string &name = "unknown"):id_(id), name_(name) {}
    ~JailUnit();
    virtual ~MapUnit() = default;
    // int getId() const { return id_; }
    // std::string getName() const  { return name_; }
    // int getStay() const { return who_is_here_; }
    void arrest(const Player &p) { p.status_ = "inJail"; }
}

#endif