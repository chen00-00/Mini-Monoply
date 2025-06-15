#ifndef MAP__
#define MAP__

#include <string>
#include <vector>
#include <iostream>

// Forward declare Player class
class Player;

// ===== MapUnit (base class) =====
class MapUnit {
protected:
  int id_ = 0;
  std::string name_;
  std::vector<Player*> playersHerePtrs_;
  std::string getPlayersHereString() const;
public:
  MapUnit(int id, const std::string& name, int numPlayers);
  virtual ~MapUnit() = default;

  virtual void onVisit(Player* player) = 0;
  virtual const std::string type() const = 0;
  virtual void reset() {}
  virtual bool isPurchasable() const { return false; }
  virtual const std::string display() const;

  const int getId() const { return id_; }
  const std::string getName() const { return name_; }

  void addPlayerHere(Player* p);
  void removePlayerHere(Player* p);
  const std::vector<Player*>& getPlayersHere() const;
};

// ================== Purchasable Unit ====================
class PurchasableUnit : public MapUnit {
protected:
    int price_ = 0;
    Player* host_ = nullptr;
    void tryToBuy(Player* player);
public:
    PurchasableUnit(int id, const std::string& name, int numPlayers, int price);
    ~PurchasableUnit() = default;

    bool isPurchasable() const override { return true; }
    const std::string display() const override;

    const int getPrice() const { return price_; }  
    const Player* getHost() const { return host_; }
    void setHost(Player* p) { host_ = p; }
};


// ================== Upgradable Unit ====================
class UpgradableUnit : public PurchasableUnit {
private:
  int upgrade_price_ = 0;
  int fines_[5] = {0};
  int level_ = 1;

public:
  UpgradableUnit(int id, const std::string& name, int numPlayers, int price, int upgrade_price, const int* fines);

  void onVisit(Player* player) override;
  const std::string type() const override;
  void reset() override;
  const std::string display() const override;

  void upgrade();
  const int getFine() const;
  const int getUpgradePrice() const;
  const int getLevel() const;
};

// ================== Random Cost Unit ====================
class RandomCostUnit : public PurchasableUnit {
private:
  int finePerPoint_ = 0;

public:
  RandomCostUnit(int id, const std::string& name, int numPlayers, int price, int finePerPoint);

  void onVisit(Player* player) override;
  const std::string type() const override;
  void reset() override;
  const std::string display() const override;
};

// ================== Collectable Unit ====================
class CollectableUnit : public PurchasableUnit {
private:
    int unitFine_ = 0;

public:
    CollectableUnit(int id, const std::string& name, int numPlayers, int price, int unitFine);
    void onVisit(Player* player) override;
    const std::string type() const override;
    void reset() override;
    const std::string display() const override;
};

// ================== Jail Unit ====================
class JailUnit : public MapUnit {
public:
    JailUnit(int id, const std::string& name, int numPlayers);
    void onVisit(Player* player) override;
    const std::string type() const override;
    const std::string display() const override;
};

// ================== World Map ====================
class WorldMap {
private:
  std::vector<MapUnit*> units_;
public:
  WorldMap(int numPlayers);
  ~WorldMap();

  MapUnit* getUnit(int index) const;
  const int getUnitCount() const;
};

#endif
