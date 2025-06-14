#ifndef MAP__
#define MAP__

#include <string>
#include <vector>
#include <iostream>

// Forward declare Player class
class Player;
constexpr int MAX_PLAYERS = 4;

// ===== MapUnit (base class) =====
class MapUnit {
protected:
  int id_;
  std::string name_;
  Player* players_here_ptrs_[MAX_PLAYERS] = {};
  std::string getPlayersHereString() const;
public:
  MapUnit(int id, const std::string& name) : id_(id), name_(name) {}
  virtual ~MapUnit() = default;

  virtual void onVisit(Player* player) = 0;
  virtual std::string type() const = 0;
  virtual void reset() {}
  virtual bool isPurchasable() const { return false; }
  virtual std::string display() const;

  int getId() const { return id_; }
  std::string getName() const { return name_; }

  void addPlayerHere(Player* p);
  void removePlayerHere(Player* p);
  Player* const* getPlayersHere() const;
};

// ================== Purchasable Unit ====================
class PurchasableUnit : public MapUnit {
protected:
    int price_;
    Player* host_ = nullptr;
    void tryToBuy(Player* player);
public:
    PurchasableUnit(int id, const std::string& name, int price);
    ~PurchasableUnit() = default;

    bool isPurchasable() const override { return true; }
    std::string display() const override;

    int getPrice() const { return price_; }  
    Player* getHost() const { return host_; }
    void setHost(Player* p) { host_ = p; }
};


// ================== Upgradable Unit ====================
class UpgradableUnit : public PurchasableUnit {
private:
  int upgrade_price_;
  int fines_[5];
  int level_;

public:
  UpgradableUnit(int id, const std::string& name, int price, int upgrade_price, const int* fines);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;
  std::string display() const override;

  void upgrade();
  int getFine() const;
  int getUpgradePrice() const;
  int getLevel() const;
};

// ================== Random Cost Unit ====================
class RandomCostUnit : public PurchasableUnit {
private:
  int fine_per_point_;

public:
  RandomCostUnit(int id, const std::string& name, int price, int fine_per_point);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;
  std::string display() const override;
};

// ================== Collectable Unit ====================
class CollectableUnit : public PurchasableUnit {
private:
    int unit_fine_;

public:
    CollectableUnit(int id, const std::string& name, int price, int unit_fine);
    void onVisit(Player* player) override;
    std::string type() const override;
    void reset() override;
    std::string display() const override;
};

// ================== Jail Unit ====================
class JailUnit : public MapUnit {
public:
    JailUnit(int id, const std::string& name);
    void onVisit(Player* player) override;
    std::string type() const override;
    std::string display() const override;
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
