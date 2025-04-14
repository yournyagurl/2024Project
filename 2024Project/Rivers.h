#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>

/**
 * @brief Manages a collection of rivers and their associated continents.
 *
 * This class is responsible for loading river data from files, providing access
 * to random rivers, and determining if two rivers belong to the same continent.
 * It also keeps track of recently selected rivers to avoid immediate repetition.
 */
class Rivers {
public:
    /**
     * @brief Constructs a Rivers object by loading river data from the specified files.
     *
     * Each file is expected to contain river names, one per line, optionally followed by a comma and the continent name.
     * If the continent is not specified, a default continent might be assigned based on the file name or some other logic
     * (implementation-dependent).
     *
     * @param filenames A constant reference to a vector of strings, where each string is the path to a data file containing river information.
     *
     * @throws FileAccessException If any of the specified files cannot be opened or read.
     *
     * @complexity Depends on the size of the input files. If there are N total rivers across all files, and the average line length is L,
     * the complexity is roughly O(N * L) to read the files and populate the internal data structures. The unordered map insertions
     * have an average time complexity of O(L) per river name.
     */
    Rivers(const std::vector<std::string>& filenames);

    /**
     * @brief Sets the operating mode of the Rivers object for additional configurations.
     *
     * The specific behavior associated with different modes is implementation-defined.
     *
     * @param mode An integer representing the desired mode.
     *
     * @complexity O(1)
     */
    void setMode(int mode);

    /**
     * @brief Gets a random river name from the collection of all available rivers.
     *
     * This method takes into account the recently selected rivers (up to `recentLimit`)
     * to avoid immediate repetition.
     *
     * @return A string representing the name of a randomly selected river.
     *
     * @complexity O(R) in the worst case, where R is the total number of rivers, if all but one are in the recent list.
     * However, on average, it should be closer to O(1) after a few selections as the probability of hitting a non-recent river increases.
     */
    std::string getRandomRiver();

    /**
     * @brief Checks if two given rivers belong to the same continent.
     *
     * @param r1 The name of the first river.
     * @param r2 The name of the second river.
     * @return True if both rivers are from the same continent, false otherwise. Returns false if either river is not found in the data.
     *
     * @complexity O(1) on average, due to the unordered map lookups.
     */
    bool sameContinent(std::string r1, std::string r2);

    /**
     * @brief Gets the continent of a given river.
     *
     * @param river The name of the river.
     * @return A string representing the continent of the river. Returns an empty string if the river is not found in the data.
     *
     * @complexity O(1) on average, due to the unordered map lookup.
     */
    std::string getContinent(std::string river);

    /**
     * @brief A vector containing the names of rivers located in Europe.
     */
    std::vector<std::string> europeRivers;

    /**
     * @brief A vector containing the names of rivers located in Asia.
     */
    std::vector<std::string> asianRivers;

    /**
     * @brief A vector containing the names of rivers located in Africa.
     */
    std::vector<std::string> africanRivers;

    /**
     * @brief A vector containing the names of rivers located in South America.
     */
    std::vector<std::string> southAmericaRivers;

    /**
     * @brief A vector keeping track of the names of recently selected rivers.
     * This is used to avoid immediate repetition in `getRandomRiver()`.
     */
    std::vector<std::string> recentRivers;

    /**
     * @brief An unordered set providing fast (average O(1)) lookup of recently selected rivers.
     */
    std::unordered_set<std::string> recentRiversSet;

    /**
     * @brief A constant representing the maximum number of recently selected rivers to keep track of.
     */
    static constexpr int recentLimit = 6;

    /**
     * @brief A static random number generator (Mersenne Twister engine) used for selecting random rivers.
     * It is initialized once for the entire class.
     */
    static std::mt19937 randomRiverGenerator;

private:
    /**
     * @brief An unordered map that associates each river name (key) with its continent (value).
     */
    std::unordered_map<std::string, std::string> riverToContinent;

    /**
     * @brief An integer storing the currently selected mode of operation.
     * Defaults to 1. The meaning of different mode values is implementation-specific.
     */
    int selectedMode = 1;
};

/**
 * @brief Custom exception class for handling errors related to file access,
 * such as when data files are empty or absent.
 */
class FileAccessException : public std::runtime_error {
public:
    /**
     * @brief Constructs a FileAccessException with a specific error message.
     *
     * @param message The error message describing the file access issue.
     */
    FileAccessException(const std::string& message) : std::runtime_error(message) {}
};