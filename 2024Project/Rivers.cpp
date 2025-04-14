#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include "Rivers.h"

Rivers::Rivers(const std::vector<std::string>& filenames) {
    // Populate continent vectors and the map of rivers to continents
    for (const auto& fileName : filenames) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file " << fileName << std::endl;
            continue; // Skip this file if it can't be opened
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue; // Skip empty lines

            if (fileName.find("Europe") != std::string::npos) {
                europeRivers.push_back(line);
                riverToContinent[line] = "Europe";
            }
            else if (fileName.find("Asia") != std::string::npos) {
                asianRivers.push_back(line);
                riverToContinent[line] = "Asia";
            }
            else if (fileName.find("Africa") != std::string::npos) {
                africanRivers.push_back(line);
                riverToContinent[line] = "Africa";
            }
            else if (fileName.find("South America") != std::string::npos) {
                southAmericaRivers.push_back(line);
                riverToContinent[line] = "South America";
            }
        }
    }
}

void Rivers::setMode(int mode) {
    selectedMode = mode;
}
std::string Rivers::getRandomRiver() {
    // Combine all rivers into a single list
    std::vector<std::string> allRivers;
    allRivers.insert(allRivers.end(), europeRivers.begin(), europeRivers.end());
    allRivers.insert(allRivers.end(), asianRivers.begin(), asianRivers.end());
    allRivers.insert(allRivers.end(), africanRivers.begin(), africanRivers.end());
    allRivers.insert(allRivers.end(), southAmericaRivers.begin(), southAmericaRivers.end());

    if (allRivers.empty()) return "Error: No rivers available";

    std::string chosenRiver;
    std::vector<std::string>* selectedContinent = nullptr;

    // Mode 1: Equal probability of each continent
    if (selectedMode == 1) {
        int continentIndex = std::uniform_int_distribution<int>(0, 3)(randomRiverGenerator);
        switch (continentIndex) {
        case 0: selectedContinent = &europeRivers; break;
        case 1: selectedContinent = &asianRivers; break;
        case 2: selectedContinent = &africanRivers; break;
        case 3: selectedContinent = &southAmericaRivers; break;
        }
    }
    // Mode 3: 50% chance of being from the same continent as the previous river
    else if (selectedMode == 3) {
        std::string lastContinent = "";
        if (!recentRivers.empty()) {
            lastContinent = getContinent(recentRivers.back());
        }

        bool chooseSameContinent = std::uniform_int_distribution<int>(0, 1)(randomRiverGenerator);
        if (chooseSameContinent && !lastContinent.empty()) {
            if (lastContinent == "Europe") selectedContinent = &europeRivers;
            else if (lastContinent == "Asia") selectedContinent = &asianRivers;
            else if (lastContinent == "Africa") selectedContinent = &africanRivers;
            else if (lastContinent == "South America") selectedContinent = &southAmericaRivers;
        }

        if (!selectedContinent) {
            int continentIndex = std::uniform_int_distribution<int>(0, 3)(randomRiverGenerator);
            switch (continentIndex) {
            case 0: selectedContinent = &europeRivers; break;
            case 1: selectedContinent = &asianRivers; break;
            case 2: selectedContinent = &africanRivers; break;
            case 3: selectedContinent = &southAmericaRivers; break;
            }
        }
    }

    if (!selectedContinent) {
        return "Error: No continent selected.";
    }

    if (selectedContinent->empty()) {
        return "Error: Selected continent has no rivers.";
    }

    int attempts = 100;  // Prevent infinite loops
    bool foundValidRiver = false;

    // Loop until a valid, non-repeated river is found
    while (!foundValidRiver && attempts > 0) {
        if (selectedContinent->empty()) {
            return "Error: No rivers available in selected continent.";
        }
        int index = std::uniform_int_distribution<int>(0, selectedContinent->size() - 1)(randomRiverGenerator);
        chosenRiver = (*selectedContinent)[index];

        // Ensure the chosen river hasn't been selected recently
        if (recentRiversSet.find(chosenRiver) == recentRiversSet.end()) {
            foundValidRiver = true;
        }

        --attempts;  // Prevent infinite loop
    }

    if (!foundValidRiver) {
        return "Error: No unique rivers available";
    }

    // Add the chosen river to recentRivers and recentRiversSet
    recentRivers.push_back(chosenRiver);
    recentRiversSet.insert(chosenRiver);

    // Maintain a rolling window of 6 unique rivers
    if (recentRivers.size() > 6) {
        std::string oldestRiver = recentRivers.front();
        recentRiversSet.erase(oldestRiver);  // Remove from the set
        recentRivers.erase(recentRivers.begin());  // Remove from the vector
    }

    return chosenRiver;
}



bool Rivers::sameContinent(std::string r1, std::string r2) {
    // Cache results to avoid redundant calls to getContinent
    std::string continent1 = getContinent(r1);
    std::string continent2 = getContinent(r2);

    if (continent1.empty() || continent2.empty()) {
        return false;
    }

    return continent1 == continent2;
}

//O(k)
std::string Rivers::getContinent(std::string river) {
    auto it = riverToContinent.find(river);
    if (it != riverToContinent.end()) {
        return it->second;
    }
    else {
        return ""; // If river is not found in any continent, return empty
    }
}

std::mt19937 Rivers::randomRiverGenerator(std::random_device{}());
