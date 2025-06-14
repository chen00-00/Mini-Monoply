#include <iostream>
#include <vector>
#include <string>
#include <iomanip>    // For std::setw and std::left
#include <limits>     // For std::numeric_limits
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()

#include "map.h"
#include "player.h"

void clearScreen();
void displayBoard(const WorldMap& map, const WorldPlayer& players);
void displayPlayerStatus(const WorldPlayer& players, int currentPlayerIndex);
int rollDice();
void handleBuyAction(Player* player, MapUnit* unit);
void handleUpgradeAction(Player* player, UpgradableUnit* unit);
void waitForEnter();

int main() {
    // Seed the random number generator using the current time for varied dice rolls.
    srand(time(0));

    // 1. Game Setup
    // Create an instance of the WorldMap, which loads map data from "map.dat".
    WorldMap worldMap;

    int numPlayers = 0;
    // Default names for players, used if the user doesn't input custom names.
    std::vector<std::string> defaultNames = {"A-Tu", "Little-Mei", "King-Baby", "Mrs.Money"};

    clearScreen(); // Clear the console screen for a clean start.

    // ==================== Handle text or numeric input logic ====================
    std::cout << "How many players?(Maximum:4)...>";
    std::cin >> numPlayers;

    // Scenario 1: Invalid input (e.g., text)
    if (std::cin.fail()) {
        numPlayers = 1; // Default to 1 player if input is invalid.
        std::cin.clear(); // Clear the error flags on std::cin.
        // Ignore the rest of the invalid input in the buffer up to the newline character.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // Scenario 2: Input is a valid number
    else {
        // Limit the number of players between 1 and 4.
        if (numPlayers > 4) {
            numPlayers = 4;
        }
        else if (numPlayers < 1) {
            numPlayers = 1;
        }

        // Clear the input buffer of any leftover newline characters before using getline.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Loop to ask for player names based on the number of players.
        for (int i = 0; i < numPlayers; ++i) {
            std::string name;
            std::cout << "Please input player " << i + 1 << "'s name (Default: " << defaultNames[i] << ")...>";
            std::getline(std::cin, name);

            if (!name.empty()) {
                defaultNames[i] = name;
            }
        }
    }

    WorldPlayer players(numPlayers, defaultNames);
    // Place all players at the starting point (location 0).
    for (int i = 0; i < numPlayers; ++i) {
        worldMap.getUnit(0)->addPlayerHere(players.playerNow(i));
    }

    clearScreen();

    // --- Initial Game State Display ---
    displayBoard(worldMap, players);
    displayPlayerStatus(players, 0);

    // 2. Main Game Loop
    int currentPlayerIndex = 0; // Initialize currentPlayerIndex to 0.
    int activePlayers = numPlayers; // Initialize activePlayers with the total number of players.

    // The game loop continues until a specific condition (e.g., only one active player or exit choice) is met.
    while (true) {
        Player* currentPlayer = players.playerNow(currentPlayerIndex); // Get the current player object.

        // If the current player is bankrupt, skip their turn and move to the next player.
        if (currentPlayer->getStatus() == PlayerStatus::Bankrupt) {
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
            continue;
        }

        // Prompt the current player for their action.
        std::cout << currentPlayer->getName() << ", your action? (1:Dice [default] / 2:Exit)...>";
        std::string choice = "";
        std::getline(std::cin, choice); // Read player's choice.

        // If the player chooses to exit, break the game loop.
        if (choice == "2") {
            break;
        }

        // If the current player is in jail, they miss a turn.
        if (currentPlayer->getStatus() == PlayerStatus::InJail) {
            std::cout << currentPlayer->getName() << " is in jail and misses a turn.";
            currentPlayer->releaseFromJail(); // Release them from jail for the next round.
            waitForEnter(); // Wait for user to press Enter.
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers; // Move to the next player.
            clearScreen(); // Clear screen and update display for the next turn.
            displayBoard(worldMap, players);
            displayPlayerStatus(players, currentPlayerIndex);
            continue; // Skip the rest of the current player's turn.
        }

        // 4. Roll Dice and Move
        int diceRoll = rollDice(); // Roll the dice to get a number between 1 and 6.

        int oldLocation = currentPlayer->getLocation(); // Get player's current location.
        // Calculate new location, wrapping around the board if necessary.
        int newLocation = (oldLocation + diceRoll) % worldMap.getUnitCount();

        // Check if the player passed "GO" (crossed the starting point).
        if (newLocation < oldLocation) {
            int reward = 2000; // Initialize reward to 2000.
            std::cout << currentPlayer->getName() << " passed GO and collects $" << reward << "!" << std::endl; // Passing start gives reward
            currentPlayer->receive(reward); // Give the player the reward.
        }
        // Move the player to the new location on the map.
        currentPlayer->moveTo(newLocation, &worldMap);

        // Get the MapUnit object at the player's new location.
        MapUnit* currentUnit = worldMap.getUnit(newLocation);

        // First, display the game board after the player has moved.
        clearScreen();
        displayBoard(worldMap, players);
        displayPlayerStatus(players, currentPlayerIndex);

        // Trigger the onVisit action for the unit the player landed on.
        currentUnit->onVisit(currentPlayer);
        // Handle actions related to buying or upgrading units.
        handleBuyAction(currentPlayer, currentUnit);

        // Check for bankruptcy after actions.
        if (currentPlayer->getMoney() < 0) { // If player's money falls below zero, they are bankrupt.
            std::cout << std::endl << currentPlayer->getName() << " is bankrupt!";
            currentPlayer->declareBankruptcy(); // Set player status to bankrupt and release all units.
            activePlayers--; // Decrease the count of active players.
        }

        waitForEnter(); // Pause execution until user presses Enter.

        // Move to the next player in the turn order.
        currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;

        // If only one player remains active, the game ends.
        if (activePlayers == 1) {
            break;
        }

        // Display board for the next turn, clearing the screen first.
        clearScreen();
        displayBoard(worldMap, players);
        displayPlayerStatus(players, currentPlayerIndex);
    }

    // 8. Announce Winner (This part would typically involve identifying the last active player)
    std::cout << "The winner is determined!";

    return 0; // End of the program.
}

/* Clear the console screen */
void clearScreen() {
    // system("cls") is for Windows; use system("clear") for Linux/macOS.
    system("cls");
}

// Rolls a dice and returns a random number between 1 and 6.
int rollDice() {
    return rand() % 6 + 1; // A random number in [1, 6]
}

// Pauses execution until the user presses the Enter key.
void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::string dummy = ""; // Initialize dummy to empty string.
    std::getline(std::cin, dummy); // Read and discard the input.
}

// Handles the logic for buying or interacting with a unit.
void handleBuyAction(Player* player, MapUnit* unit) {
    // If the unit is unowned and has a price (i.e., it's buyable).
    if (unit->getHost() == nullptr && unit->getPrice() > 0) { // Unowned and buyable
        int price = unit->getPrice();
        if (player->getMoney() >= price) { // If the player has enough money to buy.
            std::cout << player->getName() << ", do you want to buy " << unit->getName() << "? (1: Yes [default] / 2: No) ...>";
            std::string buy_choice = ""; // Initialize buy_choice to empty string.
            std::getline(std::cin, buy_choice); // Get player's choice to buy.
            if (buy_choice != "2") { // If player chooses to buy (or presses Enter for default Yes).
                player->pay(price); // Player pays the price.
                player->addUnit(unit); // Add the unit to the player's owned units.
                std::cout << "You pay $" << price << " to buy " << unit->getName();
            }
        }
    }
    // If the unit is owned by the current player, check if it's upgradable.
    else if (unit->getHost() == player) {
        // Use dynamic_cast to check if the MapUnit is an UpgradableUnit.
        if (UpgradableUnit* u_unit = dynamic_cast<UpgradableUnit*>(unit)) {
             handleUpgradeAction(player, u_unit); // If so, handle the upgrade action.
        }
    }
}

// Handles the logic for upgrading an upgradable unit.
void handleUpgradeAction(Player* player, UpgradableUnit* unit) {
    if (unit->getLevel() < 5) { // Maximum level is 5.
        int upgrade_price = unit->getUpgradePrice();
        if (player->getMoney() >= upgrade_price) { // If the player has enough money to upgrade.
             std::cout << player->getName() << ", do you want to upgrade " << unit->getName() << "? (1: Yes [default] / 2: No)...>";
             std::string upgrade_choice = ""; // Initialize upgrade_choice to empty string.
             std::getline(std::cin, upgrade_choice); // Get player's choice to upgrade.
             if(upgrade_choice != "2") { // If player chooses to upgrade.
                player->pay(upgrade_price); // Player pays the upgrade price.
                unit->upgrade(); // Upgrade the unit's level.
                std::cout << "You pay $" <<unit->getUpgradePrice() << " to upgrade " << unit->getName() << " to Lv." << unit->getLevel();
             }
        }
    }
    else { // If unit->getLevel() is 5, it's already at max level.
        std::cout << player->getName() << ", your " << unit->getName() << " already reaches the highest level!";
    }
}

// Displays the game board, showing units, owners, prices/fines, and player positions.
void displayBoard(const WorldMap& map, const WorldPlayer& players) {
    if (map.getUnitCount() == 0) return; // If there are no units on the map, do nothing.

    const int n_players = players.getPlayerCount(); // Get the total number of players.
    // Calculate half the size of the board for symmetrical display.
    int half_size = (map.getUnitCount() + 1) / 2;

    // Loop through half of the units to display both left and right sides of the board.
    for (int i = 0; i < half_size; ++i) {
        int left_id = i; // ID of the unit on the left side.
        int right_id = map.getUnitCount() - 1 - i; // ID of the unit on the right side.

        MapUnit* left_unit = map.getUnit(left_id);
        MapUnit* right_unit = (left_id != right_id) ? map.getUnit(right_id) : nullptr;

        // --- 1. Prepare player position track strings ---
        std::string left_track(n_players, ' ');
        for (int j = 0; j < n_players; ++j) {
            Player* p = players.playerNow(j);
            if (p->getLocation() == left_id) {
                left_track[p->getId()] = std::to_string(p->getId())[0];
            }
        }

        std::string right_track(n_players, ' ');
        if (right_unit) {
            for (int j = 0; j < n_players; ++j) {
                Player* p = players.playerNow(j);
                if (p->getLocation() == right_id) {
                    right_track[p->getId()] = std::to_string(p->getId())[0];
                }
            }
        }

        // --- 2. Prepare information blocks for the left property ---
        std::stringstream left_owner_ss;
        if (left_unit->getHost() != nullptr) {
            left_owner_ss << "{" << left_unit->getHost()->getId() << "}";
        }

        std::stringstream left_info_ss;
        if (left_unit->type() == "J") {
            left_info_ss << "J";
        }
        else if (left_unit->getHost() == nullptr) {
            left_info_ss << "B$ " << left_unit->getPrice();
        }
        else {
            if (UpgradableUnit* u_unit = dynamic_cast<UpgradableUnit*>(left_unit)) {
                if (u_unit->getLevel() == 5) {
                    left_info_ss << "L5";
                }
                else {
                    left_info_ss << "U$" << std::setw(5) << std::right << u_unit->getUpgradePrice() << " L" << u_unit->getLevel();
                }
            }
            else if (CollectableUnit* c_unit = dynamic_cast<CollectableUnit*>(left_unit)) {
                left_info_ss << "x" << c_unit->getHost()->getNumCollectableUnits();
            }
            else {
                left_info_ss << "?";
            }
        }

        // --- 3. Prepare information blocks for the right property ---
        std::stringstream right_owner_ss, right_info_ss;
        if (right_unit) {
            if (right_unit->getHost() != nullptr) {
                right_owner_ss << "{" << right_unit->getHost()->getId() << "}";
            }
            if (right_unit->type() == "J") {
                right_info_ss << "J";
            }
            else if (right_unit->getHost() == nullptr) {
                right_info_ss << "B$ " << right_unit->getPrice();
            }
            else {
                if (UpgradableUnit* u_unit = dynamic_cast<UpgradableUnit*>(right_unit)) {
                    if (u_unit->getLevel() == 5) {
                        right_info_ss << "L5";
                    }
                    else {
                        right_info_ss << "U$" <<  std::setw(5) << std::right << u_unit->getUpgradePrice() << " L" << u_unit->getLevel();
                    }
                }
                else if (CollectableUnit* c_unit = dynamic_cast<CollectableUnit*>(right_unit)) {
                    right_info_ss << "x" << c_unit->getHost()->getNumCollectableUnits();
                }
                else {
                    right_info_ss << "?";
                }
            }
        }

        // --- 4. Final output formatting ---
        std::cout << "=" << std::setw(n_players) << std::left << left_track << "=  "
                  << "[" << left_id << "]"
                  << std::setw(10) << std::right << left_unit->getName().substr(0, 10)
                  << " " << std::setw(4) << std::left << left_owner_ss.str()
                  << std::setw(14) << std::left << left_info_ss.str();

        if (right_unit) {
            std::cout << "="
                      << std::setw(n_players) << std::left << right_track << "=  "
                      << "[" << right_id << "]"
                      << std::setw(10) << std::right << right_unit->getName().substr(0, 10)
                      << " " << std::setw(4) << std::left << right_owner_ss.str()
                      << std::setw(14) << std::left << right_info_ss.str();
        }

        std::cout << std::endl;
    }
}

// Displays the status of all active players.
void displayPlayerStatus(const WorldPlayer& players, int currentPlayerIndex) {
    std::cout << std::endl;
    for (int i = 0; i < players.getPlayerCount(); ++i) {
        const auto* p = players.playerNow(i);
        if (p->getStatus() == PlayerStatus::Bankrupt) {
            continue;
        }

        if (i == currentPlayerIndex) std::cout << "=>";
        else std::cout << "  ";

        std::cout << "[" << p->getId() << "]  " << std::setw(15) << std::right << p->getName().substr(0, 15)
                  << "  $" << std::setw(6) << std::left << p->getMoney()
                  << "with " << p->getUnitCount() << " units" << std::endl;
    }
    std::cout << std::endl;
}
