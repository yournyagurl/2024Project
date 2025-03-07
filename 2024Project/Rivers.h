#pragma once
#include <string>
#include <vector>
#include <queue>

class Rivers
{
	//Insert data structure(s) here...
	std::vector<std::string> asianRivers;
	std::vector<std::string> europeRivers;
	std::vector<std::string> southAmericaRivers;
	std::vector<std::string> africanRivers;
	std::vector<std::string> recentRivers;
	int selectedMode = 1;

public:
	//Time complexity: O(n)
	Rivers(const std::vector<std::string>& filenames);
	//Time complexity:	O(1)
	std::string getRandomRiver();
	//Time complexity: O(n)
	bool sameContinent(std::string river1, std::string river2);
	//Time complexity: O(n)
	std::string getContinent(std::string river);

	//No need to implement this until assignment 2
	void setMode(int mode) {
		selectedMode = mode;
	}


};