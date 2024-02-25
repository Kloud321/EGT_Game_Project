#include "FileHandler.h"
#include <fstream>

void FileHandler::SaveScore(int score) {
    std::ofstream file("score.txt", std::ios_base::app);
    if (file.is_open()) {
        file << score << std::endl;
        file.close();
    }
}

std::vector<int> FileHandler::LoadScores() {
    std::vector<int> scores;
    std::ifstream file("score.txt");
    int score;
    if (file.is_open()) {
        while (file >> score) {
            scores.push_back(score);
        }
        file.close();
    }
    return scores;
}