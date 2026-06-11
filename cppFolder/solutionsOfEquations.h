#pragma once

#include <vector>
#include <utility> 

struct SeidelIteration {
    int iter;
    std::vector<double> X;
    double diff;
};

std::vector<std::vector<double>> multiply(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
std::pair<std::vector<double>, std::vector<double>> luSolve(const std::vector<std::vector<double>>& A, const std::vector<double>& B);
std::vector<SeidelIteration> seidelSolve(const std::vector<std::vector<double>>& A, const std::vector<double>& B, double eps);

void printLuResults(const std::pair<std::vector<double>, std::vector<double>>& luResult);
void printSeidelResults(const std::vector<SeidelIteration>& history);
void runSolver();