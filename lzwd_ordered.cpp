#include <iostream>
#include <map>
#include <string>
#include <vector>

// Function to perform LZW compression
std::vector<int> compressLZW(const std::string& input) {
    std::map<std::string, int> dictionary;
    std::vector<int> compressed;

    // Initialize the dictionary with individual symbols
    int code = 1;
    for (char c : input) {
        std::string symbol(1, c);
        if (dictionary.find(symbol) == dictionary.end())
            dictionary[symbol] = code++;
    }

    std::string currentPattern = std::string(1, input[0]);
    int currentIndex = 0;

    while (currentIndex < input.length()) {
        char nextChar = input[currentIndex + 1];
        std::string pattern = std::string(1, input[currentIndex + 1]);

        int i = 0;
        while (dictionary.find(pattern) != dictionary.end()) {
            // Pattern already exists in the dictionary
            pattern = pattern + input[currentIndex + i];
            i++;
        }
        std::cout << "pattern: " << pattern << std::endl;
        // Output the code for the current pattern
        compressed.push_back(dictionary[currentPattern + pattern]);

        // Insert the new pattern into the dictionary
        dictionary[currentPattern + pattern] = code++;
        currentPattern = pattern;
        // std::cout << "currentpattern: " << dictionary[currentPattern] << std::endl;
        currentIndex++;
    }

    // Output the code for the last pattern
    compressed.push_back(dictionary[currentPattern]);

    /* // Print the dictionary
    std::cout << "Dictionary:" << std::endl;
    for (const auto& entry : dictionary) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    } */

    return compressed;
}

int main() {
    std::string input = "AABBABABAAABBBABABBBAA";
    std::vector<int> compressed = compressLZW(input);

    std::cout << "Compressed codes: ";
    for (int code : compressed) {
        std::cout << code << " ";
    }
    std::cout << std::endl;

    return 0;
}
