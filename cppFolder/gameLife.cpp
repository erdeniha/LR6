#include "gameLife.h"
#include "getRandomInt.h"
#include "gameLife.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>

void clearGrid(GameState& state) {
    state.grid.assign(state.rows, std::vector<int>(state.cols, 0));
}

void initRandom(GameState& state, int fillPercentage) {
    clearGrid(state);
    for (int i = 0; i < state.rows; ++i) {
        for (int j = 0; j < state.cols; ++j) {
            if (getRandomInt(1, 100) <= fillPercentage) {
                state.grid[i][j] = 1;
            }
        }
    }
}

void putGlider(GameState& state, int r, int c) {
    const int patternRows = 3;
    const int patternCols = 3;
    const int pattern[patternRows][patternCols] = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };

    if (r + patternRows <= state.rows && c + patternCols <= state.cols) {
        for (int i = 0; i < patternRows; ++i) {
            for (int j = 0; j < patternCols; ++j) {
                if (pattern[i][j] == 1) {
                    state.grid[r + i][c + j] = 1;
                }
            }
        }
    }
}

void putLightweightSpaceship(GameState& state, int r, int c) {
    const int patternRows = 4;
    const int patternCols = 5;
    const int pattern[patternRows][patternCols] = {
        {0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0}
    };

    if (r + patternRows <= state.rows && c + patternCols <= state.cols) {
        for (int i = 0; i < patternRows; ++i) {
            for (int j = 0; j < patternCols; ++j) {
                if (pattern[i][j] == 1) {
                    state.grid[r + i][c + j] = 1;
                }
            }
        }
    }
}
void putMiddleweightSpaceship(GameState& state, int r, int c) {
    const int patternRows = 5;
    const int patternCols = 6;
    const int pattern[patternRows][patternCols] = {
        {0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0}
    };

    if (r + patternRows <= state.rows && c + patternCols <= state.cols) {
        for (int i = 0; i < patternRows; ++i) {
            for (int j = 0; j < patternCols; ++j) {
                if (pattern[i][j] == 1) {
                    state.grid[r + i][c + j] = 1;
                }
            }
        }
    }
}

void putHeavyweightSpaceship(GameState& state, int r, int c) {
    const int patternRows = 5;
    const int patternCols = 7;
    const int pattern[patternRows][patternCols] = {
        {0, 0, 0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 0}
    };

    if (r + patternRows <= state.rows && c + patternCols <= state.cols) {
        for (int i = 0; i < patternRows; ++i) {
            for (int j = 0; j < patternCols; ++j) {
                if (pattern[i][j] == 1) {
                    state.grid[r + i][c + j] = 1;
                }
            }
        }
    }
}

void putQueenBeeShuttle(GameState& state, int r, int c) {
    const std::vector<std::pair<int, int>> aliveCells = {
        {0,0}, {0,1}, {0,20}, {0,21},
        {1,0}, {1,1}, {1,8}, {1,13}, {1,20}, {1,21},
        {2,7}, {2,9}, {2,12}, {2,14},
        {3,7}, {3,10}, {3,11}, {3,14},
        {4,8}, {4,9}, {4,12}, {4,13},
        {5,10}, {5,11},
        {6,9}, {6,12}
    };

    for (const auto& cell : aliveCells) {
        int targetR = r + cell.first;
        int targetC = c + cell.second;
        
        if (targetR < state.rows && targetC < state.cols) {
            state.grid[targetR][targetC] = 1;
        }
    }
}

int countAliveNeighbors(const GameState& state, int r, int c) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;

            int nr = (r + i + state.rows) % state.rows;
            int nc = (c + j + state.cols) % state.cols;
            
            count += state.grid[nr][nc];
        }
    }
    return count;
}

void nextGeneration(GameState& state) {
    std::vector<std::vector<int>> newGrid = state.grid;

    for (int i = 0; i < state.rows; ++i) {
        for (int j = 0; j < state.cols; ++j) {
            int alive = countAliveNeighbors(state, i, j);
            if (state.grid[i][j] == 1) {
                if (alive < 2 || alive > 3) newGrid[i][j] = 0;
            } else {
                if (alive == 3) newGrid[i][j] = 1;
            }
        }
    }
    state.grid = newGrid;
}

void printState(const GameState& state) {
    std::system("clear"); 
    
    for (int i = 0; i < state.rows; ++i) {
        for (int j = 0; j < state.cols; ++j) {
            std::cout << (state.grid[i][j] ? "O " : ". ");
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

void runGameOfLife(GameState& state, int delayMs) {
    while (true) {
        printState(state);
        nextGeneration(state);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void gameLife() {
    GameState state;
    state.rows = ROWS;
    state.cols = COLS;
    clearGrid(state);

    int choice;
    std::cout << "1. Random\n2. Glider\n3. Lightweight Spaceship\n4. Middleweight Spaceship\n5. Heavyweight Spaceship\n6. Queen Bee Shuttle\n";
    std::cin >> choice;

    switch(choice) {
        case 1:
            initRandom(state, 15);
            break;
        case 2: 
            putGlider(state, 2, 2);
            break;
        case 3:
            putLightweightSpaceship(state, 5, 5);
            break;
        case 4:
            putMiddleweightSpaceship(state, 5, 5);
            break;
        case 5:
            putHeavyweightSpaceship(state, 5, 5);
            break;
        case 6:
            putQueenBeeShuttle(state, 10, 5);
            break;
        default:
            initRandom(state, 15);
    }

    runGameOfLife(state, 150);
}