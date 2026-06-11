#include "initializingMatrix.h"
#include "getRandomInt.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>

void initializingMatrix() {
    int M;
    int N;
    std::cout << "Введите размерность матрицы:";
    std::cin >> M >> N;


    std::vector<std::vector<int>> matrix(M, std::vector<int>(N));
    double totalSum = 0;
    int totalElements = M * N;

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = getRandomInt(0, 100);
            totalSum += matrix[i][j];
            std::cout << std::setw(4) << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nСреднее арифметическое всех элементов матрицы: " << (totalSum / totalElements) << "\n";

    double maxRowMean = -1;
    int maxRowIndex = -1;

    for (int i = 0; i < M; ++i) {
        double rowSum = std::accumulate(matrix[i].begin(), matrix[i].end(), 0.0);
        double rowMean = rowSum / N;
        std::cout << "Строка №" << (i+1) << " среднее арифметическое: " << rowMean << "\n";

        if (rowMean > maxRowMean) {
            maxRowMean = rowMean;
            maxRowIndex = i;
        }
    }

    std::cout << "Индекс строки с максимальным средним арифметическим: " << maxRowIndex 
    << " (Среднее арифметическое: " << maxRowMean << ")\n";
}