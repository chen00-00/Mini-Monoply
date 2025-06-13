#ifndef PLAYER__
#define PLAYER__

#include <iostream>
#include <vector>
#include <string>

// Forward declare MapUnit & WorldMap class
class MapUnit;
class WorldMap;

enum class PlayerStatus { Normal, InJail, Bankrupt };

// ================== Player ==================
class Player {
public:
    Player(int id = 0, const std::string& name = "nameless");
    ~Player() {}

    int getId() const;
    const std::string& getName() const;
    int getMoney() const;
    int getLocation() const;
    PlayerStatus getStatus() const;
    int getUnitCount() const;
    int getNumCollectableUnits() const;


    //void pay(Player* player, int amount);
    int pay(int amount);
    void receive(int amount);
    void moveTo(int new_location, WorldMap* map);
    void addUnit(MapUnit* unit);
    void releaseAllUnits();
    void setToJail();
    void releaseFromJail();
    void declareBankruptcy();

private:
    int id_ = 0;
    std::string name_;
    int location_ = 0;
    int money_ = 30000;
    std::vector<MapUnit*> owned_units_;
    PlayerStatus status_ = PlayerStatus::Normal; // Normal, InJail, Bankruptcy
};

// ================== World Player ==================
class WorldPlayer {
public:
  WorldPlayer(int num_player, std::vector<std::string>& Names);
  ~WorldPlayer();

  Player* playerNow(int index) const;
  int getPlayerCount() const;
private:
  std::vector<Player*> players_;
};

#endif
