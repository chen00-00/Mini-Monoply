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

class MapUnit {
public:
    MapUnit(int id = -1, const std::string& name = "unknown") : id_(id), name_(name) {}
    virtual ~MapUnit() = default;

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    int getStayPlayer() const { return who_is_here_; }
    virtual void onPlayerEnter(Player& p) = 0;

protected:
    int id_;
    std::string name_;
    int who_is_here_ = -1;
};

class CollectableUnit : public MapUnit {
public:
    CollectableUnit(int id = -1, const std::string &name = "unknown", int price = 0, int fine = 0):MapUnit(id, name), price_(price), fine_(fine) {}
    ~CollectableUnit() = default;
    int getPrice() const  { return price_; }
    int getFine() const  { return fine_ * num_cunits_; }
    int getHost() const { return host_; }
    int getCUM() const { return num_cunits_; }
    void updateHost(const Player &p) { host_ = p.getId(); num_cunits_ = p.getCUM(); }
    void reset() { host_ = -1; num_cunits_ = 0; }
private:
    int price_ = 0;
    int fine_ = 0;
    int host_ = -1;         // default: -1(nobody)
    int num_cunits_ = 0;    // number of the CollectableUnit that the host has
};

class JailUnit : public MapUnit {
public:
    JailUnit(int id = -1, const std::string &name = "Jail"):MapUnit(id, name) {}
    ~JailUnit() = default;
    void onPlayerEnter(Player& p) override {
        p.updateStatus(Status::InJail);
    }
};

#endif