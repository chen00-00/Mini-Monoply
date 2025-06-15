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
    Player(int id, const std::string& name);
    ~Player() {}

    const int getId() const;
    const std::string& getName() const;
    const int getMoney() const;
    const int getLocation() const;
    const PlayerStatus getStatus() const;
    const int getUnitCount() const;
    const int getNumCollectableUnits() const;


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
    PlayerStatus status_ = PlayerStatus::Normal; // Normal, InJail, Bankrupt
};

// ================== World Player ==================
class WorldPlayer {
public:
  WorldPlayer(int num_player, std::vector<std::string>& Names);
  ~WorldPlayer();

  Player* playerNow(int index) const;
  const int getPlayerCount() const;
private:
  std::vector<Player*> players_;
};

#endif
