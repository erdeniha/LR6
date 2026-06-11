#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <clocale>

void printMask(int mask, int N, int M) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int bit_pos = i * M + j;
            int bit_value = (mask >> bit_pos) & 1;
            std::cout << bit_value << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::pair<int, int> getTargetMasks(int N, int M) {
    int target1 = 0;
    int target2 = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int bit_pos = i * M + j;
            if ((i + j) % 2 == 0) {
                target1 |= (1 << bit_pos);
            } else {
                target2 |= (1 << bit_pos);
            }
        }
    }
    return {target1, target2};
}

std::vector<int> solveChessboard(int N, int M, int start_mask) {
    auto [target1, target2] = getTargetMasks(N, M);

    if (start_mask == target1 || start_mask == target2) {
        return {start_mask};
    }

    std::queue<int> q;
    std::map<int, int> dist;   
    std::map<int, int> parent; 

    q.push(start_mask);
    dist[start_mask] = 0;
    parent[start_mask] = -1;

    int end_mask = -1;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        int d = dist[curr];

        if (curr == target1 || curr == target2) {
            end_mask = curr;
            break;
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                int pos1 = i * M + j;

                if (j + 1 < M) {
                    int pos2 = i * M + (j + 1);
                    int next_mask = curr ^ (1 << pos1) ^ (1 << pos2);
                    if (dist.find(next_mask) == dist.end()) {
                        dist[next_mask] = d + 1;
                        parent[next_mask] = curr;
                        q.push(next_mask);
                    }
                }

                if (i + 1 < N) {
                    int pos2 = (i + 1) * M + j;
                    int next_mask = curr ^ (1 << pos1) ^ (1 << pos2);
                    if (dist.find(next_mask) == dist.end()) {
                        dist[next_mask] = d + 1;
                        parent[next_mask] = curr;
                        q.push(next_mask);
                    }
                }
            }
        }
    }

    if (end_mask == -1) {
        return {};
    }

    std::vector<int> path;
    int curr_node = end_mask;
    while (curr_node != -1) {
        path.push_back(curr_node);
        curr_node = parent[curr_node];
    }
    std::reverse(path.begin(), path.end());

    return path;
}

void runChessboardSolver() {
    int N, M;
    std::cout << "Введите размеры доски N и M: ";
    if (!(std::cin >> N >> M)) return;

    std::cout << "Введите матрицу состояний (0 - орел, 1 - решка):\n";
    int start_mask = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int val;
            std::cin >> val;
            if (val == 1) {
                start_mask |= (1 << (i * M + j));
            }
        }
    }

    std::vector<int> path = solveChessboard(N, M, start_mask);

    if (path.empty()) {
        std::cout << "\nРешения не существует\n";
        return;
    }

    if (path.size() == 1) {
        std::cout << "\nНачальное состояние уже является целевым!\n";
        printMask(path[0], N, M);
        std::cout << "Минимальное число ходов: 0\n";
        return;
    }

    std::cout << "\nПромежуточные состояния доски\n\n";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i == 0) {
            std::cout << "Начальное состояние:\n";
        } else {
            std::cout << "Ход №" << i << ":\n";
        }
        printMask(path[i], N, M);
    }

    std::cout << "Минимальное число ходов: " << path.size() - 1 << "\n";
}
