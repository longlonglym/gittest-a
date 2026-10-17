#include "sudoku.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>

// ==================== SudokuGrid ====================

SudokuGrid::SudokuGrid() { clear(); }

void SudokuGrid::clear() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      grid[i][j] = 0;
    }
  }
}

void SudokuGrid::print() const {
  std::cout << "\n  ╔═══════╤═══════╤═══════╗\n";
  for (int i = 0; i < 9; i++) {
    std::cout << "  ║ ";
    for (int j = 0; j < 9; j++) {
      if (grid[i][j] == 0) {
        std::cout << ". ";
      } else {
        std::cout << grid[i][j] << " ";
      }
      if (j % 3 == 2 && j < 8) {
        std::cout << "│ ";
      }
    }
    std::cout << "║\n";
    if (i % 3 == 2 && i < 8) {
      std::cout << "  ╟───────┼───────┼───────╢\n";
    }
  }
  std::cout << "  ╚═══════╧═══════╧═══════╝\n\n";
}

bool SudokuGrid::isValid(int row, int col, int num) const {
  // 检查行
  for (int j = 0; j < 9; j++) {
    if (grid[row][j] == num) return false;
  }
  // 检查列
  for (int i = 0; i < 9; i++) {
    if (grid[i][col] == num) return false;
  }
  // 检查 3x3 宫
  int boxRow = (row / 3) * 3;
  int boxCol = (col / 3) * 3;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[boxRow + i][boxCol + j] == num) return false;
    }
  }
  return true;
}

bool SudokuGrid::isComplete() const {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (grid[i][j] == 0) return false;
    }
  }
  return true;
}

// ==================== SudokuSolver ====================

bool SudokuSolver::solve(int grid[9][9]) { return solveHelper(grid); }

bool SudokuSolver::solveHelper(int grid[9][9]) {
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (grid[row][col] == 0) {
        // 尝试 1-9
        for (int num = 1; num <= 9; num++) {
          // 内联合法性检查（避免创建对象）
          bool valid = true;
          // 行
          for (int j = 0; j < 9; j++) {
            if (grid[row][j] == num) {
              valid = false;
              break;
            }
          }
          if (!valid) continue;
          // 列
          for (int i = 0; i < 9; i++) {
            if (grid[i][col] == num) {
              valid = false;
              break;
            }
          }
          if (!valid) continue;
          // 宫
          int br = (row / 3) * 3, bc = (col / 3) * 3;
          for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
              if (grid[br + i][bc + j] == num) {
                valid = false;
                break;
              }
            }
            if (!valid) break;
          }
          if (!valid) continue;

          grid[row][col] = num;
          if (solveHelper(grid)) return true;
          grid[row][col] = 0;
        }
        return false;
      }
    }
  }
  return true;
}

int SudokuSolver::countSolutions(int grid[9][9], int limit) {
  int count = 0;
  countHelper(grid, limit, count);
  return count;
}

void SudokuSolver::countHelper(int grid[9][9], int limit, int& count) {
  if (count >= limit) return;

  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (grid[row][col] == 0) {
        for (int num = 1; num <= 9; num++) {
          // 合法性检查
          bool valid = true;
          for (int j = 0; j < 9; j++) {
            if (grid[row][j] == num) {
              valid = false;
              break;
            }
          }
          if (!valid) continue;
          for (int i = 0; i < 9; i++) {
            if (grid[i][col] == num) {
              valid = false;
              break;
            }
          }
          if (!valid) continue;
          int br = (row / 3) * 3, bc = (col / 3) * 3;
          for (int i = 0; i < 3 && valid; i++) {
            for (int j = 0; j < 3; j++) {
              if (grid[br + i][bc + j] == num) {
                valid = false;
                break;
              }
            }
          }
          if (!valid) continue;

          grid[row][col] = num;
          countHelper(grid, limit, count);
          grid[row][col] = 0;
        }
        return;
      }
    }
  }
  // 找到一个解
  count++;
}

// ==================== SudokuGenerator ====================

void SudokuGenerator::fillBoard(int grid[9][9]) {
  // 先清空
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) grid[i][j] = 0;
  fillHelper(grid);
}

bool SudokuGenerator::fillHelper(int grid[9][9]) {
  // 寻找空格子
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (grid[row][col] == 0) {
        // 随机排列 1-9
        std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        static std::mt19937 rng(std::time(nullptr));
        std::shuffle(nums.begin(), nums.end(), rng);

        for (int num : nums) {
          // 合法性检查
          bool valid = true;
          for (int j = 0; j < 9; j++)
            if (grid[row][j] == num) {
              valid = false;
              break;
            }
          if (!valid) continue;
          for (int i = 0; i < 9; i++)
            if (grid[i][col] == num) {
              valid = false;
              break;
            }
          if (!valid) continue;
          int br = (row / 3) * 3, bc = (col / 3) * 3;
          for (int i = 0; i < 3 && valid; i++)
            for (int j = 0; j < 3; j++)
              if (grid[br + i][bc + j] == num) {
                valid = false;
                break;
              }
          if (!valid) continue;

          grid[row][col] = num;
          if (fillHelper(grid)) return true;
          grid[row][col] = 0;
        }
        return false;
      }
    }
  }
  return true;
}

void SudokuGenerator::removeCells(int grid[9][9], Difficulty difficulty) {
  int cellsToRemove;
  switch (difficulty) {
    case Difficulty::EASY:
      cellsToRemove = 32;
      break;  // 保留 49 个
    case Difficulty::MEDIUM:
      cellsToRemove = 42;
      break;  // 保留 39 个
    case Difficulty::HARD:
      cellsToRemove = 52;
      break;  // 保留 29 个
  }

  // 生成所有格子的坐标列表，随机打乱
  std::vector<std::pair<int, int>> cells;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) cells.push_back({i, j});

  static std::mt19937 rng(std::time(nullptr) + 12345);
  std::shuffle(cells.begin(), cells.end(), rng);

  int removed = 0;
  for (auto& cell : cells) {
    if (removed >= cellsToRemove) break;

    int row = cell.first, col = cell.second;
    int backup = grid[row][col];
    grid[row][col] = 0;

    // 验证是否仍有唯一解
    int tempGrid[9][9];
    std::memcpy(tempGrid, grid, sizeof(tempGrid));
    if (solver.countSolutions(tempGrid, 2) == 1) {
      removed++;
    } else {
      // 恢复
      grid[row][col] = backup;
    }
  }
}

void SudokuGenerator::generate(int puzzle[9][9], int solution[9][9],
                               Difficulty difficulty) {
  // 1. 填满棋盘
  fillBoard(solution);

  // 2. 复制到谜面
  std::memcpy(puzzle, solution, 81 * sizeof(int));

  // 3. 根据难度移除格子
  removeCells(puzzle, difficulty);
}
