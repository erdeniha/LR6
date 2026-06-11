#include <iostream>
#include "initializingMatrix.h"
#include "initializingSquareMatrix.h"
#include "gameLife.h"
#include "AES.h"
#include "solutionsOfEquations.h"
#include "coinBoard.h"

int main() {
    int choice;
    do {
        std::cout << "\n1. Матрица\n2. Квадратная матрица\n3. Автомат Джона Конвея\n4. AES (OFB)\n5. Система уравнений\n6. Доска с монетами\n0. Выход\n";

        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case 1:
                initializingMatrix();
                break;
            case 2:
                initializingSquareMatrix();
                break;
            case 3:
                gameLife();
                break;
            case 4:
                AES();
                break;
            case 5:
                runSolver();
                break;
            case 6:
                runChessboardSolver();
                break;               
            case 0:
                break;
            default:
                std::cout << "Неверный ввод.\n";
        }
    } while (choice != 0);

    return 0;
}