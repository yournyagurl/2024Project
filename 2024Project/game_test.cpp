#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE GameTest
#include <boost/test/included/unit_test.hpp>
#include "Game.h"
#include "Rivers.h"
#include <sstream>
#include <chrono>

// Helper fixture class to set up Game instance
struct GameFixture {
    Rivers rivers;
    Game game;

    GameFixture() : rivers({ "Europe_test.txt", "Asia_test.txt", "Africa_test.txt", "South_America_test.txt" }), game(rivers) {}

    // Method to mock user input and capture output
    void mockInputAndCaptureOutput(const std::string& userInput, std::ostream& out) {
        std::stringstream ssInput(userInput);
        game.playRound(out, ssInput);
    }
};

BOOST_FIXTURE_TEST_SUITE(GameTestSuite, GameFixture)

// Test Case 1: Valid correct answer (same continent)
BOOST_AUTO_TEST_CASE(TestCorrectSameContinentAnswer) {
    std::stringstream output;
    mockInputAndCaptureOutput("s", output);

    BOOST_CHECK(output.str().find("correct") != std::string::npos);
    BOOST_CHECK(output.str().find("Danube = Europe") != std::string::npos);
    BOOST_CHECK(output.str().find("Yangtze = Asia") == std::string::npos);
}

// Test Case 2: Valid incorrect answer (same continent)
BOOST_AUTO_TEST_CASE(TestIncorrectSameContinentAnswer) {
    std::stringstream output;
    mockInputAndCaptureOutput("s", output);

    BOOST_CHECK(output.str().find("incorrect") != std::string::npos);
}

// Test Case 3: Valid correct answer (different continent)
BOOST_AUTO_TEST_CASE(TestCorrectDifferentContinentAnswer) {
    std::stringstream output;
    mockInputAndCaptureOutput("d", output);

    BOOST_CHECK(output.str().find("correct") != std::string::npos);
    BOOST_CHECK(output.str().find("Danube = Europe") != std::string::npos);
    BOOST_CHECK(output.str().find("Yangtze = Asia") == std::string::npos);
}

// Test Case 4: Valid incorrect answer (different continent)
BOOST_AUTO_TEST_CASE(TestIncorrectDifferentContinentAnswer) {
    std::stringstream output;
    mockInputAndCaptureOutput("d", output);

    BOOST_CHECK(output.str().find("incorrect") != std::string::npos);
}

// Test Case 5: Timeout (elapsed time exceeds 10 seconds)
BOOST_AUTO_TEST_CASE(TestTimeout) {
    std::stringstream output;
    // Simulate user input with delay (more than 10 seconds)
    mockInputAndCaptureOutput("s", output);
    BOOST_CHECK(output.str().find("Time limit exceeded!") != std::string::npos);
}

// Test Case 6: User chooses to quit the game
BOOST_AUTO_TEST_CASE(TestQuitOption) {
    std::stringstream output;
    std::stringstream ssInput("q\n");
    bool result = game.playRound(output, ssInput);
    BOOST_CHECK(!result); // The game should stop on 'q' input
}

// Test Case 7: Invalid input (anything other than 's' or 'd')
BOOST_AUTO_TEST_CASE(TestInvalidInput) {
    std::stringstream output;
    std::stringstream ssInput("x\n");  // Invalid input
    mockInputAndCaptureOutput("x", output);
    BOOST_CHECK(output.str().find("incorrect") != std::string::npos);
}

// Test Case 8: Timeout with invalid answer input
BOOST_AUTO_TEST_CASE(TestTimeoutInvalidInput) {
    std::stringstream output;
    std::stringstream ssInput("y\n");  // Invalid answer but within 10 seconds
    game.playRound(output, ssInput);
    BOOST_CHECK(output.str().find("incorrect") != std::string::npos);
}

// Test Case 9: Checking if score is updated correctly
BOOST_AUTO_TEST_CASE(TestScoreUpdateCorrectAnswer) {
    std::stringstream output;
    std::stringstream ssInput("s\n");  // Correct answer, same continent
    mockInputAndCaptureOutput("s", output);
    BOOST_CHECK(output.str().find("correct") != std::string::npos);
    BOOST_CHECK(output.str().find("Danube = Europe") != std::string::npos);
    BOOST_CHECK(output.str().find("Yangtze = Asia") == std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
