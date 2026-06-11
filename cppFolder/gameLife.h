#pragma once
#include <vector>

const int ROWS = 20;
const int COLS = 40;

struct GameState {
    int rows;
    int cols;
    std::vector<std::vector<int>> grid;
};

void gameLife();
