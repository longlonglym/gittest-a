#include <cstdlib>
#include <cstring>
#include <iostream>

#include "sudoku.h"

// 颜色代码（ANSI）
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
#define COLOR_MAGENTA "\033[35m"

void printBanner() {
  std::cout << COLOR_CYAN;
  std::cout << R"(
  ╔══════════════════════════════╗
  ║       数 独 游 戏           ║
  ║      Sudoku Game            ║
  ╚══════════════════════════════╝
)" << COLOR_RESET;
}

void printMenu() {
  std::cout << COLOR_YELLOW;
  std::cout << "  ┌─────────────────────────┐\n";
  std::cout << "  │  1. 生成新题目          │\n";
  std::cout << "  │  2. 输入题目求解        │\n";
  std::cout << "  │  3. 查看当前答案        │\n";
  std::cout << "  │  4. 退出                │\n";
  std::cout << "  └─────────────────────────┘\n";
  std::cout << COLOR_RESET;
  std::cout << "  请选择 (1-4): ";
}

Difficulty selectDifficulty() {
  int choice;
  std::cout << "\n  选择难度:\n";
  std::cout << "  1. 简单 (Easy)\n";
  std::cout << "  2. 中等 (Medium)\n";
  std::cout << "  3. 困难 (Hard)\n";
  std::cout << "  请选择 (1-3): ";
  std::cin >> choice;
  switch (choice) {
    case 1:
      return Difficulty::EASY;
    case 2:
      return Difficulty::MEDIUM;
    case 3:
      return Difficulty::HARD;
    default:
      return Difficulty::MEDIUM;
  }
}

void readPuzzle(int grid[9][9]) {
  std::cout << "\n  请输入数独题目（9行，每行9个数字，空格用 0 或 . 表示）:\n";
  std::cout << "  示例: 530070000600195000098000060...\n\n";

  // 清空输入缓冲
  std::cin.ignore();

  for (int i = 0; i < 9; i++) {
    std::cout << "  第 " << (i + 1) << " 行: ";
    std::string line;
    std::getline(std::cin, line);

    for (int j = 0; j < 9 && j < (int)line.length(); j++) {
      if (line[j] == '.' || line[j] == '0') {
        grid[i][j] = 0;
      } else if (line[j] >= '1' && line[j] <= '9') {
        grid[i][j] = line[j] - '0';
      } else {
        grid[i][j] = 0;
      }
    }
  }
}

int main() {
// 设置控制台为 UTF-8（Windows）
#ifdef _WIN32
  system("chcp 65001 > nul");
#endif

  SudokuGenerator generator;
  SudokuSolver solver;
  SudokuGrid display;

  int puzzle[9][9] = {0};
  int solution[9][9] = {0};
  bool hasPuzzle = false;
  bool hasSolution = false;

  while (true) {
    printBanner();
    printMenu();

    int choice;
    std::cin >> choice;

    switch (choice) {
      case 1: {
        // 生成新题目
        Difficulty diff = selectDifficulty();
        std::cout << "\n  " << COLOR_GREEN << "正在生成数独题目..."
                  << COLOR_RESET << "\n";

        generator.generate(puzzle, solution, diff);

        hasPuzzle = true;
        hasSolution = true;

        std::memcpy(display.grid, puzzle, sizeof(puzzle));
        std::cout << COLOR_BOLD << "\n  📝 题目:\n" << COLOR_RESET;
        display.print();

        // 显示难度和空格数
        int emptyCells = 0;
        for (int i = 0; i < 9; i++)
          for (int j = 0; j < 9; j++)
            if (puzzle[i][j] == 0) emptyCells++;
        std::cout << "  空格数: " << emptyCells << "\n\n";
        break;
      }
      case 2: {
        // 输入题目求解
        int inputGrid[9][9];
        std::memset(inputGrid, 0, sizeof(inputGrid));
        readPuzzle(inputGrid);

        std::memcpy(display.grid, inputGrid, sizeof(inputGrid));
        std::cout << COLOR_BOLD << "\n  📝 输入的题目:\n" << COLOR_RESET;
        display.print();

        if (solver.solve(inputGrid)) {
          hasPuzzle = true;
          hasSolution = true;
          std::memcpy(puzzle, display.grid, sizeof(puzzle));
          std::memcpy(solution, inputGrid, sizeof(solution));

          std::memcpy(display.grid, solution, sizeof(solution));
          std::cout << COLOR_GREEN << "  ✅ 求解成功！\n" << COLOR_RESET;
          std::cout << COLOR_BOLD << "\n  🎯 答案:\n" << COLOR_RESET;
          display.print();
        } else {
          std::cout << COLOR_MAGENTA << "  ❌ 该题目无解，请检查输入。\n"
                    << COLOR_RESET;
        }
        break;
      }
      case 3: {
        // 查看当前答案
        if (!hasSolution) {
          std::cout << COLOR_MAGENTA
                    << "\n  ⚠️  暂无答案，请先生成题目或求解题目。\n\n"
                    << COLOR_RESET;
        } else {
          std::memcpy(display.grid, solution, sizeof(solution));
          std::cout << COLOR_BOLD << "\n  🎯 答案:\n" << COLOR_RESET;
          display.print();
        }
        break;
      }
      case 4: {
        std::cout << COLOR_CYAN << "\n  再见！\n" << COLOR_RESET;
        return 0;
      }
      default:
        std::cout << "\n  ⚠️  无效选择，请重新输入。\n\n";
        break;
    }
  }

  return 0;
}
