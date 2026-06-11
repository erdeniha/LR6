#include "solutionsOfEquations.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <utility>
#include <vector>

std::vector<std::vector<double>> multiply(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
    int numberRowsA = A.size();
    int numberRowsB = B[0].size();
    int numberColsA = A[0].size();
    std::vector<std::vector<double>> matrix(numberRowsA, std::vector<double>(numberRowsB, 0.0));
    for (int i = 0; i < numberRowsA; ++i)
        for (int j = 0; j < numberRowsB; ++j)
            for (int k = 0; k < numberColsA; ++k)
                matrix[i][j] += A[i][k] * B[k][j];
    return matrix;
}


std::pair<std::vector<double>, std::vector<double>> luSolve(const std::vector<std::vector<double>>& A, const std::vector<double>& B) {
    int n = A.size();
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    std::vector<std::vector<double>> U(n, std::vector<double>(n, 0.0));
    
    for (int i = 0; i < n; ++i) {
        L[i][i] = 1.0;
        for (int j = i; j < n; ++j) {
            double s = 0.0;
            for (int k = 0; k < i; ++k) s += L[i][k] * U[k][j];
            U[i][j] = A[i][j] - s;
        }
        for (int j = i + 1; j < n; ++j) {
            double s = 0.0;
            for (int k = 0; k < i; ++k) s += L[j][k] * U[k][i];
            L[j][i] = (A[j][i] - s) / U[i][i];
        }
    }

    std::vector<double> Y(n, 0.0);
    for (int i = 0; i < n; ++i) {
        double s = 0.0;
        for (int j = 0; j < i; ++j) s += L[i][j] * Y[j];
        Y[i] = B[i] - s;
    }

    std::vector<double> X(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        double s = 0.0;
        for (int j = i + 1; j < n; ++j) s += U[i][j] * X[j];
        X[i] = (Y[i] - s) / U[i][i];
    }

    return {Y, X};
}

std::vector<SeidelIteration> seidelSolve(const std::vector<std::vector<double>>& A, const std::vector<double>& B, double eps) {
    int n = A.size();
    std::vector<std::vector<double>> AT(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) 
        for (int j = 0; j < n; ++j) 
            AT[i][j] = A[j][i];
    
    std::vector<std::vector<double>> Anew = multiply(AT, A);
    std::vector<std::vector<double>> Bm(n, std::vector<double>(1));
    for (int i = 0; i < n; ++i) Bm[i][0] = B[i];
    
    std::vector<std::vector<double>> Bnewm = multiply(AT, Bm);

    std::vector<double> Bnew(n);
    for (int i = 0; i < n; ++i) Bnew[i] = Bnewm[i][0];

    std::vector<double> X(n, 0.0);
    double diff = eps + 1.0;
    int iter = 0;

    std::vector<SeidelIteration> history;

    while (diff > eps) {
        diff = 0.0;
        for (int i = 0; i < n; ++i) {
            double old = X[i];
            double s = 0.0;
            for (int j = 0; j < n; ++j) if (i != j) s += Anew[i][j] * X[j];
            X[i] = (Bnew[i] - s) / Anew[i][i];
            if (std::abs(X[i] - old) > diff) diff = std::abs(X[i] - old);
        }
        iter++;
        
        history.push_back({iter, X, diff});
    }
    
    return history;
}

void printLuResults(const std::pair<std::vector<double>, std::vector<double>>& luResult) {
    std::cout << "Прямой ход метода LU\n";
    for (size_t i = 0; i < luResult.first.size(); ++i) {
        std::cout << "y" << i + 1 << " = " << luResult.first[i] << "\n";
    }

    std::cout << "\nОбратный ход метода LU\n";
    for (size_t i = 0; i < luResult.second.size(); ++i) {
        std::cout << "x" << i + 1 << " = " << luResult.second[i] << "\n";
    }
}

void printSeidelResults(const std::vector<SeidelIteration>& history) {
    std::cout << "\nИтерационный метод Зейделя\n";
    std::cout << std::setw(5) << "N" << " | "
              << std::setw(10) << "X1" << " | "
              << std::setw(10) << "X2" << " | "
              << std::setw(10) << "X3" << " | "
              << std::setw(10) << "X4" << " | "
              << std::setw(10) << "En" << "\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& step : history) {
        std::cout << std::setw(5) << step.iter << " | "
                  << std::fixed << std::setprecision(5)
                  << std::setw(10) << step.X[0] << " | "
                  << std::setw(10) << step.X[1] << " | "
                  << std::setw(10) << step.X[2] << " | "
                  << std::setw(10) << step.X[3] << " | "
                  << std::setw(10) << step.diff << "\n";
    }
}

void runSolver() {
    const double M = -1.19, N_val = -0.21, P = 1.21;
    const std::vector<std::vector<double>> A = {
        {M, -0.04, 0.21, -1.16},
        {0.25, -1.23, N_val, -0.09},
        {-0.21, N_val, 0.8, -0.13},
        {0.15, -1.31, 0.06, P}
    };
    const std::vector<double> B = {-1.24, P, 2.56, M};
    const double eps = 1e-3;
    auto luResult = luSolve(A, B);
    printLuResults(luResult);
    
    auto seidelHistory = seidelSolve(A, B, eps);
    printSeidelResults(seidelHistory);
}