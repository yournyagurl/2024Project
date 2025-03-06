#include "Game.h"
#include <chrono>

bool Game::playRound(std::ostream& out, std::istream& in) {
    // Get two random rivers and print them as required
    std::string r1 = r.getRandomRiver();
    std::string r2 = r.getRandomRiver();

    out << r1 << std::endl;
    out << r2 << std::endl;

    // Start measuring response time
    auto start = std::chrono::steady_clock::now();

    std::string userInput;
    in >> userInput;

    auto end = std::chrono::steady_clock::now();
    int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // If time exceeded 10,000 ms (10 seconds), show timeout message and return true
    if (elapsedTime > 10000) {
        out << "Time limit exceeded!" << std::endl;
        totalScore++;
        return true;
    }

    // Quit option
    if (userInput == "q") {
        return false;
    }

    bool isCorrect = false;

    // Check if the user pressed 's' (same continent)
    if (userInput == "s") {
        isCorrect = (r.getContinent(r1) == r.getContinent(r2));
    }
    // Check if the user pressed 'd' (different continent)
    else if (userInput == "d") {
        isCorrect = (r.getContinent(r1) != r.getContinent(r2));
    }

    if (userInput == "s" || userInput == "d") {
        if (isCorrect) {
            out << "correct" << std::endl;
            score++;
        }
        else {
            out << "incorrect" << std::endl;
        }

        out << r1 << " = " << r.getContinent(r1) << std::endl;
        out << r2 << " = " << r.getContinent(r2) << std::endl;
        totalScore++;

        // Record correct answers with response time
        if (isCorrect) {
            recordCorrectAnswer(r1, r2, elapsedTime);
        }

        return true;
    }

    // Invalid input is treated as incorrect
    totalScore++;
    out << "incorrect" << std::endl;
    return true;
}
