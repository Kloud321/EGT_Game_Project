#include "FileHandler.h"
#include <fstream>

void FileHandler::SaveScore(int score) {
    std::ofstream file("score.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}