#include "Rivers.h"
#include <fstream>
#include <random>
#include <iostream>

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

// random river generation 

std::string Rivers::getRandomRiver() {
	static std::mt19937 randomRiverGenerator(std::random_device{}());

	std::vector<std::string> availableRivers;
	availableRivers.insert(availableRivers.end(), europeRivers.begin(), europeRivers.end());
	availableRivers.insert(availableRivers.end(), asianRivers.begin(), asianRivers.end());
	availableRivers.insert(availableRivers.end(), africanRivers.begin(), africanRivers.end());
	availableRivers.insert(availableRivers.end(), southAmericaRivers.begin(), southAmericaRivers.end());

	std::vector<std::string> validRivers;
	for (const auto& river : availableRivers) {
		if (std::find(recentRivers.begin(), recentRivers.end(), river) == recentRivers.end()) {
			validRivers.push_back(river);
		}
	}

	if (validRivers.empty()) {
		validRivers = availableRivers;
		recentRivers.clear();
	}

	std::uniform_int_distribution<int> distribution(0, validRivers.size() - 1);
	std::string selectedRiver = validRivers[distribution(randomRiverGenerator)];

	recentRivers.push_back(selectedRiver);
	if (recentRivers.size() > 6) {
		recentRivers.erase(recentRivers.begin());
	}

	return selectedRiver;
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