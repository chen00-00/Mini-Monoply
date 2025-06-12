#ifndef MAP__
#define MAP__

#include <string>
#include <vector>
#include <memory>
#include <iostream>

class Player;


// ================== MapUnit (base class) ==================
class MapUnit {
protected:
  int id_;
  std::string name_;
  Player* host_ = nullptr;
  std::vector<Player*> who_is_here_;

public:
  MapUnit(int id, const std::string& name) : id_(id), name_(name) {}
  virtual ~MapUnit() = default;

  virtual void onVisit(Player* player) = 0;
  virtual std::string type() const = 0;
  virtual void reset() {}
  
  int getId() const { return id_; }
  std::string getName() const { return name_; }
  Player* getHost() const { return host_; }
  void setHost(Player* p) { host_ = p; }

  void addPlayerHere(Player* p);
  void removePlayerHere(Player* p);
  const std::vector<Player*>& getPlayersHere()const;
};


// ================== Upgradable Unit ====================
class UpgradableUnit : public MapUnit {
private:
  int price_;
  int upgrade_price_;
  int fines_[5];
  int level_;

public:
  UpgradableUnit(int id, const std::string& name, int price, int upgrade_price, const int* fines);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;
  
  void upgrade();
  int getFine() const;
};
 
// ================== Random Cost Unit ==================
class RandomCostUnit : public MapUnit {
private:
  int price_;
  int fine_per_point_;

public:
  RandomCostUnit(int id, const std::string& name, int price, int fine_per_point);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;

  int getPrice() const { return price_; }
  int getFinePerPoint() const { return fine_per_point_; }
};

// ================== Collectable Unit ==================
class CollectableUnit : public MapUnit {
public:
  CollectableUnit(int id = -1, const std::string &name = "unknown", int price = 0, const int fine = 0):MapUnit(id, name), price_(price), fine_(fine) {}
  ~CollectableUnit() = default;
  
  int getPrice() const  { return price_; }
  int getFine() const  { return fine_ * host_->getCUM(); }
  int getCUM() const { return host_->getCUM(); }

  void onVisit(Player* player) override {
    if(getHost()){
      player->pay(getHost(), getFine());
    }
    else {
      int choose = 1;
      std::cout << player->getName() << ", do you want to buy " << getName() << "? (1: Yes [default] / 2: No) ...>" << std::endl;
      std::cin >> choose;
      if(choose == 1) {
        player->pay(getPrice());
        setHost(player);
        std::cout << "You pay $" << getPrice() <<" to buy " << getName();
      }
    }
  }
  std::string type() const override { return "C"; }
  void reset() override { host_ = nullptr; }

private:
  int price_ = 0;
  int fine_ = 0;
};

// ================== Jail Unit ==================
class JailUnit : public MapUnit {
public:
  JailUnit(int id = -1, const std::string &name = "Jail"):MapUnit(id, name) {}
  ~JailUnit() = default;

  void onVisit(Player* player) override {
      player->updateStatus(Status::InJail);
  }
  std::string type() const override { return "J"; }
};

// ================== World Map ====================
class WorldMap {
private:
  std::vector<MapUnit*> units_;
public:
  WorldMap();
  ~WorldMap();

  MapUnit* getUnit(int index) const;
  int getUnitCount() const;
};

#endif  