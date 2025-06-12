#ifndef PLAYER__
#define PLAYER__

#include <string>
#include <vector>

// Forward declaration to avoid circular dependency with map.h
class MapUnit;

// Defines the possible states a player can be in.
enum class PlayerStatus {
    Normal,
    InJail,
    Bankrupt
};

class Player {
public:
    Player(int id, const std::string& name);

    // --- Getters ---
    int getId() const;
    const std::string& getName() const;
    int getMoney() const;
    int getLocation() const;
    PlayerStatus getStatus() const;
    int getUnitCount() const;
    int getNumCollectableUnits() const;

    // --- Player Actions ---
    void pay(int amount);
    void receive(int amount);
    void moveTo(int new_location, class WorldMap* map); // Needs map to update who_is_here
    void addUnit(MapUnit* unit);
    void releaseAllUnits();

    // --- Status Changes ---
    void setToJail();
    void releaseFromJail();
    void declareBankruptcy();

private:
    int id_;
    std::string name_;
    int money_ = 30000;   // Players start with $30000
    int location_ = 0;    // All players start at location 0
    PlayerStatus status_ = PlayerStatus::Normal;
    std::vector<MapUnit*> owned_units_;
};

#endif
