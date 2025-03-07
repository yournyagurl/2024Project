#include "Rivers.h"
#include <fstream>
#include <random>
#include <iostream>
#include <unordered_set>

Rivers::Rivers(const std::vector<std::string>& filenames) 

// reading the files for the different rivers in each continent
{
	for (int i = 0; i < filenames.size(); ++i) {
		std::string fileName = filenames[i];
		std::string line;

		std::ifstream file(fileName);
		while (std::getline(file, line)) {
			if (fileName == "Europe.txt") {
				europeRivers.push_back(line);
			}
			else if (fileName == "Asia.txt") {
				asianRivers.push_back(line);
			}
			else if (fileName == "Africa.txt") {
				africanRivers.push_back(line);
			}
			else if (fileName == "South America.txt") {
				southAmericaRivers.push_back(line);
			}
		}

		file.close();
	}
}


std::string Rivers::getRandomRiver() {
	static std::mt19937 randomRiverGenerator(std::random_device{}());

	if (selectedMode == 1) {
		// Step 1: Choose a continent with equal probability
		std::vector<std::vector<std::string>> continents = { europeRivers, asianRivers, africanRivers, southAmericaRivers };
		std::vector<std::string> continentNames = { "Europe", "Asia", "Africa", "South America" };

		std::uniform_int_distribution<int> continentDist(0, continents.size() - 1);
		int chosenContinentIndex = continentDist(randomRiverGenerator);

		// Step 2: Pick a river from the chosen continent
		std::vector<std::string>& chosenContinent = continents[chosenContinentIndex];

		if (chosenContinent.empty()) return "Error: No rivers available in " + continentNames[chosenContinentIndex];

		std::uniform_int_distribution<int> riverDist(0, chosenContinent.size() - 1);
		return chosenContinent[riverDist(randomRiverGenerator)];
	}

	else if (selectedMode == 2) {
		// Completely random river selection (preserving overall distribution)
		std::vector<std::string> allRivers;
		allRivers.insert(allRivers.end(), europeRivers.begin(), europeRivers.end());
		allRivers.insert(allRivers.end(), asianRivers.begin(), asianRivers.end());
		allRivers.insert(allRivers.end(), africanRivers.begin(), africanRivers.end());
		allRivers.insert(allRivers.end(), southAmericaRivers.begin(), southAmericaRivers.end());

		if (allRivers.empty()) return "Error: No rivers available";

		std::uniform_int_distribution<int> dist(0, allRivers.size() - 1);
		return allRivers[dist(randomRiverGenerator)];
	}

	else if (selectedMode == 3) {
		// 50% chance for consecutive rivers to be from the same continent
		static std::string lastRiver = "";
		std::string lastContinent = lastRiver.empty() ? "" : getContinent(lastRiver);

		bool keepSameContinent = std::uniform_int_distribution<int>(0, 1)(randomRiverGenerator) == 0; // 50% chance

		std::vector<std::string> sameContinentRivers;
		if (lastContinent == "Europe") sameContinentRivers = europeRivers;
		else if (lastContinent == "Asia") sameContinentRivers = asianRivers;
		else if (lastContinent == "Africa") sameContinentRivers = africanRivers;
		else if (lastContinent == "South America") sameContinentRivers = southAmericaRivers;

		if (keepSameContinent && !sameContinentRivers.empty()) {
			std::uniform_int_distribution<int> dist(0, sameContinentRivers.size() - 1);
			lastRiver = sameContinentRivers[dist(randomRiverGenerator)];
			return lastRiver;
		}

		// If we cannot keep the same continent or there are no rivers, pick randomly
		std::vector<std::string> allRivers;
		allRivers.insert(allRivers.end(), europeRivers.begin(), europeRivers.end());
		allRivers.insert(allRivers.end(), asianRivers.begin(), asianRivers.end());
		allRivers.insert(allRivers.end(), africanRivers.begin(), africanRivers.end());
		allRivers.insert(allRivers.end(), southAmericaRivers.begin(), southAmericaRivers.end());

		if (allRivers.empty()) return "Error: No rivers available";

		std::uniform_int_distribution<int> dist(0, allRivers.size() - 1);
		lastRiver = allRivers[dist(randomRiverGenerator)];
		return lastRiver;
	}

	return "Error: Invalid mode";
}




bool Rivers::sameContinent(std::string r1, std::string r2)
{
	std::string r1Result = getContinent(r1);
	std::string r2Result = getContinent(r2);

	if (r1Result == "" || r2Result == "") {
		return false;
	}

	return r1Result == r2Result;
}

std::string Rivers::getContinent(std::string river)
{

	if (std::find(europeRivers.begin(), europeRivers.end(), river) != europeRivers.end()) {
		return "Europe";
	}
	else if (std::find(asianRivers.begin(), asianRivers.end(), river) != asianRivers.end()) {
		return "Asia";
	}
	else if (std::find(africanRivers.begin(), africanRivers.end(), river) != africanRivers.end()) {
		return "Africa";
	}
	else if (std::find(southAmericaRivers.begin(), southAmericaRivers.end(), river) != southAmericaRivers.end()) {
		return "South America";
	}
	else {
		return "";
	}
}