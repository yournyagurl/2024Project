#pragma once
#include "Rivers.h"
#include <ostream>
#include <istream>
#define ASSIGNMENT_2				//uncomment to switch to assignment 2
#define RUN_PAUSE_TEST  		
#include <algorithm>
//uncomment to run pause test (for assignment 2 only)


class Game {
	Rivers r;
	//further variables...
	int score;
	int totalScore;
	std::vector<std::tuple<std::string, std::string, int>> correctAnswers;


public:
	Game(Rivers& rivers) : r(rivers) {}
	int getScore() { return score; }
	int getTotal() { return totalScore; }
	//reset score and total to 0
	void reset() {
		score = 0;
		totalScore = 0;
		correctAnswers.clear();
	}

	void recordCorrectAnswer(const std::string& river1, const std::string& river2, int time) {
		correctAnswers.emplace_back(river1, river2, time);
	}

	//this method should implement one round of the game only, 
	//it should return true if the player wishes to continue ('s' or 'd') and false otherwise ('q')
	bool playRound(std::ostream& out, std::istream& in);

	//No need to implement this until assignment 2 
	std::vector<std::string> getFastest() {
		if (correctAnswers.empty()) {
			return {}; // Return an empty list if no correct answers exist
		}

		std::vector<std::tuple<std::string, std::string, int>> sortedAnswers = correctAnswers;

		// Sort by time in ascending order (slowest last)
		std::sort(sortedAnswers.begin(), sortedAnswers.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);  // Sort by increasing time
			});

		std::vector<std::string> result;
		for (size_t i = 0; i < std::min(sortedAnswers.size(), size_t(5)); ++i) {
			result.push_back(std::get<0>(sortedAnswers[i]) + "," +
				std::get<1>(sortedAnswers[i]) + "," +
				std::to_string(std::get<2>(sortedAnswers[i])));
		}
		return result;
	}
};