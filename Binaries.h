#include <iostream>
#include <vector>
#include <string>
using namespace std;

void generateSumVector(const vector<vector<uint64_t>>& vectors, vector<uint64_t>& result, int currentIndex, vector<uint64_t>& currentSum) {
    if (currentIndex == vectors.size()) {
        int sum = 0;
        for (int num : currentSum) {
            sum += num;
        }
        result.push_back(sum);
        return;
    }
    for (int num : vectors[currentIndex]) {
        currentSum.push_back(num);
        generateSumVector(vectors, result, currentIndex + 1, currentSum);
        currentSum.pop_back();
    }
    generateSumVector(vectors, result, currentIndex + 1, currentSum);
}

void generateAllStrings(std::vector<std::string>& result, std::string currentString, size_t index) {
    if (index == currentString.length()) {
        result.push_back(currentString);
        return;
    }
    if (currentString[index] == '1') {
        generateAllStrings(result, currentString, index + 1); // Do not replace '1' with '0'
        currentString[index] = '0';
        generateAllStrings(result, currentString, index + 1); // Replace '1' with '0'
    } else {
        generateAllStrings(result, currentString, index + 1);
    }
}
