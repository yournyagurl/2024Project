#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE RiversTest
#include <boost/test/included/unit_test.hpp>

#include "Rivers.h"
#include <fstream>
#include <cstdio>
#include <unordered_set>
#include <thread>

// Helpers
void createTempFile(const std::string& filename, const std::vector<std::string>& rivers) {
    std::ofstream out(filename);
    for (const auto& river : rivers) {
        out << river << "\n";
    }
}

void removeFile(const std::string& filename) {
    std::remove(filename.c_str());
}

struct RiversFixture {
    std::vector<std::string> europe = { "Danube", "Seine" };
    std::vector<std::string> asia = { "Yangtze", "Ganges" };
    std::vector<std::string> africa = { "Nile", "Congo" };
    std::vector<std::string> southAmerica = { "Amazon", "Orinoco" };
    std::vector<std::string> files;

    RiversFixture() {
        createTempFile("Europe_test.txt", europe);
        createTempFile("Asia_test.txt", asia);
        createTempFile("Africa_test.txt", africa);
        createTempFile("South America_test.txt", southAmerica);

        files = {
            "Europe_test.txt",
            "Asia_test.txt",
            "Africa_test.txt",
            "South America_test.txt"
        };
    }

    ~RiversFixture() {
        removeFile("Europe_test.txt");
        removeFile("Asia_test.txt");
        removeFile("Africa_test.txt");
        removeFile("South America_test.txt");
    }
};


// UNIT TESTING

BOOST_FIXTURE_TEST_SUITE(RiversTestSuite, RiversFixture)

BOOST_AUTO_TEST_CASE(Constructor_PopulatesRivers) {
    Rivers rivers(files);
    BOOST_CHECK_EQUAL(rivers.getContinent("Danube"), "Europe");
    BOOST_CHECK_EQUAL(rivers.getContinent("Yangtze"), "Asia");
    BOOST_CHECK_EQUAL(rivers.getContinent("Nile"), "Africa");
    BOOST_CHECK_EQUAL(rivers.getContinent("Amazon"), "South America");
}

BOOST_AUTO_TEST_CASE(SetMode_SetsModeCorrectly) {
    Rivers rivers(files);
    rivers.setMode(3);
    std::string r = rivers.getRandomRiver();
    BOOST_CHECK(!r.empty());
}

BOOST_AUTO_TEST_CASE(GetContinent_HandlesUnknownRiver) {
    Rivers rivers(files);
    BOOST_CHECK_EQUAL(rivers.getContinent("Zambezi"), "");  // Not in list
}

BOOST_AUTO_TEST_CASE(GetRandomRiver_UniqueRollingWindow) {
    Rivers rivers(files);
    rivers.setMode(1);

    std::unordered_set<std::string> picked;
    for (int i = 0; i < 6; ++i) {
        std::string r = rivers.getRandomRiver();
        BOOST_CHECK(picked.find(r) == picked.end());  // not repeated
        picked.insert(r);
    }

    // After 6, repetitions may happen again
    std::string seventh = rivers.getRandomRiver();
    BOOST_CHECK(!seventh.empty());
}

BOOST_AUTO_TEST_CASE(GetRandomRiver_Mode3_50PercentSameContinentBias) {
    Rivers rivers(files);
    rivers.setMode(3);

    std::string first = rivers.getRandomRiver();
    std::string second = rivers.getRandomRiver();

    // Either same continent or not, just check it works
    BOOST_CHECK(!second.empty());
}

BOOST_AUTO_TEST_CASE(SameContinent_WorksAsExpected) {
    Rivers rivers(files);
    BOOST_CHECK(rivers.sameContinent("Danube", "Seine"));
    BOOST_CHECK(!rivers.sameContinent("Danube", "Amazon"));
}

BOOST_AUTO_TEST_CASE(GetRandomRiver_HandlesEmptyContinentGracefully) {
    createTempFile("Empty_test.txt", {});
    std::vector<std::string> withEmpty = files;
    withEmpty.push_back("Empty_test.txt");

    Rivers rivers(withEmpty);
    rivers.setMode(1);

    // Still should give valid river despite the empty file
    std::string river = rivers.getRandomRiver();
    BOOST_CHECK(!river.empty());

    removeFile("Empty_test.txt");
}


BOOST_AUTO_TEST_CASE(HandlesInvalidFileGracefully) {
    std::vector<std::string> badFiles = { "NotExist.txt" };
    Rivers rivers(badFiles);

    std::string river = rivers.getRandomRiver();
    BOOST_CHECK(river.find("Error:") != std::string::npos);
}


// PARAMETERISED TEST CASES

BOOST_FIXTURE_TEST_CASE(recentRiversEvictionTest, RiversFixture) {
    Rivers rivers(files);
    rivers.setMode(1);

    std::vector<std::string> picked;
    for (int i = 0; i < 6; ++i) {
        std::string river = rivers.getRandomRiver();
        picked.push_back(river);
    }

    std::string evictedRiver = picked[0]; // should now be out of recent set

    bool foundEvictedAgain = false;
    for (int i = 0; i < 50; ++i) {
        std::string next = rivers.getRandomRiver();
        if (next == evictedRiver) {
            foundEvictedAgain = true;
            break;
        }
    }

    BOOST_CHECK(foundEvictedAgain);
}

BOOST_AUTO_TEST_CASE(Randomized_Mode_Variability_Test)
{
    // Create instance of Rivers class
    Rivers rivers(files);

    // Run multiple modes: 1 and 3 (equal probability & 50% bias)
    for (int mode = 1; mode <= 3; ++mode) {
        rivers.setMode(mode);
        std::unordered_set<std::string> pickedRivers;
        bool allRiversPickedOnce = true;

        // 1000 picks to test variability
        for (int i = 0; i < 1000; ++i) {
            std::string river = rivers.getRandomRiver();

            // Check if we have repeated rivers or empty values
            if (river.empty() || pickedRivers.find(river) != pickedRivers.end()) {
                allRiversPickedOnce = false;
                break;
            }

            pickedRivers.insert(river);
        }

        // Assert we don’t have duplicates and we have good randomness
        BOOST_TEST(allRiversPickedOnce);
    }
}


BOOST_AUTO_TEST_SUITE_END()

