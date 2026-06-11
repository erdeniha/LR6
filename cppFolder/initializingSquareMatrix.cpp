#include "initializingSquareMatrix.h"
#include "getRandomInt.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

void initializingSquareMatrix() {
    std::cout << "Введите размерность квадратной матрицы:";
    int N;
    std::cin >> N; 
    if (N <= 5) {
        std::cout << "Ошибка: N должно быть больше 5." << std::endl;
        return; 
    }

    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = getRandomInt(-100, 100);
            std::cout << std::setw(5) << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::vector<std::vector<int>> newMatrix(N, std::vector<int>(N));

    for (int j = 0; j < N; ++j) {
        std::vector<int> col(N);
        for (int i = 0; i < N; ++i) {
            col[i] = matrix[i][j];
        }

        if (j % 2 == 0) {
            std::sort(col.begin(), col.end());
        } else {
            std::sort(col.rbegin(), col.rend());
        }

        for (int i = 0; i < N; ++i) {
            newMatrix[i][j] = col[i];
        }
    }

    std::cout << "\nОтсортированна матрица:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << std::setw(5) << newMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}