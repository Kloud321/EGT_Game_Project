#pragma once
#include <vector>

class FileHandler {
public:
    void SaveScore(int score);
    std::vector<int> LoadScores();
};
