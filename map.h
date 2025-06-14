#ifndef MAP__
#define MAP__

#include <string>
#include <vector>
#include <memory>
#include <iostream>

// Forward declare Player class
class Player;


// ===== MapUnit (base class) =====
class MapUnit {
protected:
  int id_ = 0;
  std::string name_;
  Player* host_ = nullptr;
  std::vector<Player*> who_is_here_;

public:
  MapUnit(int id, const std::string& name) : id_(id), name_(name) {}
  virtual ~MapUnit() = default;

  virtual void onVisit(Player* player) = 0;
  virtual std::string type() const = 0;
  virtual void reset() {}
  virtual int getPrice() const { return 0; } // Universal price getter

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
  int price_ = 0;
  int upgrade_price_ = 0;
  int fines_[5] = {0};
  int level_ = 0;

public:
  UpgradableUnit(int id, const std::string& name, int price, int upgrade_price, const int* fines);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;
  int getPrice() const override;

  void upgrade();
  int getFine() const;
  int getUpgradePrice() const;
  int getLevel() const;
};

// ================== Random Cost Unit ====================
class RandomCostUnit : public MapUnit {
private:
  int price_ = 0;
  int fine_per_point_ = 0;

public:
  RandomCostUnit(int id, const std::string& name, int price, int fine_per_point);

  void onVisit(Player* player) override;
  std::string type() const override;
  void reset() override;
  int getPrice() const override;
};

// ================== Collectable Unit ====================
class CollectableUnit : public MapUnit {
private:
    int price_ = 0;
    int unit_fine_ = 0;

public:
    CollectableUnit(int id, const std::string& name, int price, int unit_fine);
    void onVisit(Player* player) override;
    std::string type() const override;
    void reset() override;
    int getPrice() const override;
};

// ================== Jail Unit ====================
class JailUnit : public MapUnit {
public:
    JailUnit(int id, const std::string& name);
    void onVisit(Player* player) override;
    std::string type() const override;
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
