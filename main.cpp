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
void waitForEnter();

int main() {
    srand(time(0));

    // 1. Game Setup
    int numPlayers = 0;
    // Default names for players, used if the user doesn't input custom names.
    std::vector<std::string> defaultNames = {"A-Tu", "Little-Mei", "King-Baby", "Mrs.Money"};

    clearScreen();

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

<<<<<<< HEAD
    // Create an instance of the WorldMap, which loads map data from "map.dat".
    WorldMap worldMap(numPlayers);

=======
>>>>>>> 89735f505a3045b4546f665e9d7b132c9692fb25
    // Create WorldPlayer for manage all players
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
    int currentPlayerIndex = 0;
    int activePlayers = numPlayers;

    // The game loop continues until a specific condition (e.g., only one active player or exit choice) is met.
    while (true) {
        Player* currentPlayer = players.playerNow(currentPlayerIndex);

        // If the current player is bankrupt, skip their turn and move to the next player.
        if (currentPlayer->getStatus() == PlayerStatus::Bankrupt) {
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
<<<<<<< HEAD
            clearScreen();
=======
            clearScreen(); // Clear screen and update display for the next turn.
>>>>>>> 89735f505a3045b4546f665e9d7b132c9692fb25
            displayBoard(worldMap, players);
            displayPlayerStatus(players, currentPlayerIndex);
            continue;
        }

        // Prompt the current player for their action.
        std::cout << currentPlayer->getName() << ", your action? (1:Dice [default] / 2:Exit)...>";
        std::string choice = "";
        std::getline(std::cin, choice);

        // If the player chooses to exit, break the game loop.
        if (choice == "2") {
            break;
        }

        // If the current player is in jail, they miss a turn.
        if (currentPlayer->getStatus() == PlayerStatus::InJail) {
            std::cout << currentPlayer->getName() << " is in jail and misses a turn.";
            currentPlayer->releaseFromJail(); // Release them from jail for the next round.
            waitForEnter();
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
            clearScreen();
            displayBoard(worldMap, players);
            displayPlayerStatus(players, currentPlayerIndex);
            continue; // Skip the rest of the current player's turn.
        }

        // 4. Roll Dice and Move
        int diceRoll = rollDice();

        int oldLocation = currentPlayer->getLocation();
        int newLocation = (oldLocation + diceRoll) % worldMap.getUnitCount();

        // Check if the player passed "GO" (crossed the starting point).
        if (newLocation < oldLocation) {
<<<<<<< HEAD
            int reward = 2000; // Initialize reward to 2000.
            currentPlayer->receive(reward);
=======
            int reward = /*200*/0; // Initialize reward to 2000.
            std::cout << currentPlayer->getName() << " passed GO and collects $" << reward << "!" << std::endl; // Passing start gives reward
            currentPlayer->receive(reward); // Give the player the reward.
>>>>>>> 89735f505a3045b4546f665e9d7b132c9692fb25
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

        // Check for bankruptcy after actions.
        if (currentPlayer->getMoney() < 0) {
            std::cout << std::endl << currentPlayer->getName() << " is bankrupt!";
            currentPlayer->declareBankruptcy();
            activePlayers--;
        }

        waitForEnter();

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

<<<<<<< HEAD
    std::cout << "The winner is determined!" ;
=======
    // 8. Announce Winner (This part would typically involve identifying the last active player)
    std::cout << "The winner is determined!" << std::endl;
>>>>>>> 89735f505a3045b4546f665e9d7b132c9692fb25

    return 0;
}

/* Clear the console screen */
void clearScreen() {
<<<<<<< HEAD
#ifdef _WIN32
    system("cls");     // Windows
#else
    system("clear");   // Linux/macOS
#endif
=======
    // system("cls") is for Windows; use system("clear") for Linux/macOS.
    system("clear");
>>>>>>> 89735f505a3045b4546f665e9d7b132c9692fb25
}

// Rolls a dice and returns a random number between 1 and 6.
int rollDice() {
    return rand() % 6 + 1;
}

// Pauses execution until the user presses the Enter key.
void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::string dummy = "";
    std::getline(std::cin, dummy);
}


// Displays the game board, showing units, owners, prices/fines, and player positions.
void displayBoard(const WorldMap& map, const WorldPlayer& players) {
    if (map.getUnitCount() == 0) return; // If there are no units on the map, do nothing.

    const int n_players = players.getPlayerCount(); // Get the total number of players.
    // Calculate half the size of the board for symmetrical display.
    int half_size = (map.getUnitCount() + 1) / 2;

    int map_size = map.getUnitCount();
    if (map.getUnitCount() % 2 == 1) {
        std::cout << std::setw(40) << std::left << map.getUnit(0)->display() << std::endl;
    }
    else {
        std::cout << std::setw(40) << std::left << map.getUnit(0)->display();
        std::cout << std::setw(40) << std::left << map.getUnit(map_size-1)->display();
        std::cout <<  std::endl;
    }
    // Loop through half of the units to display both left and right sides of the board.
    for (int i = 1; i < half_size; ++i) {
        std::cout << std::setw(40) << std::left << map.getUnit(i)->display();
        std::cout << std::setw(40) << std::left << map.getUnit(map_size-1-i)->display();
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
