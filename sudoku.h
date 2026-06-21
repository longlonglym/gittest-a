#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <utility>

// 难度等级
enum class Difficulty {
    EASY,    // 保留 46-51 个格子
    MEDIUM,  // 保留 36-45 个格子
    HARD     // 保留 26-35 个格子
};

class SudokuGrid {
public:
    int grid[9][9];

    SudokuGrid();

    // 打印棋盘（带格式化边框）
    void print() const;

    // 检查在 (row, col) 填入 num 是否合法
    bool isValid(int row, int col, int num) const;

    // 检查棋盘是否已填满且合法
    bool isComplete() const;

    // 清空棋盘
    void clear();
};

class SudokuSolver {
public:
    // 求解数独，返回是否成功（原地修改 grid）
    bool solve(int grid[9][9]);

    // 计数解的数量，最多计到 limit 个
    int countSolutions(int grid[9][9], int limit);

private:
    bool solveHelper(int grid[9][9]);
    void countHelper(int grid[9][9], int limit, int &count);
};

class SudokuGenerator {
public:
    // 生成一个完整的数独棋盘
    void fillBoard(int grid[9][9]);

    // 根据难度移除格子生成题目
    void removeCells(int grid[9][9], Difficulty difficulty);

    // 生成数独题目（包含谜面和答案）
    // puzzle: 输出的谜面（有空格）
    // solution: 输出的完整答案
    void generate(int puzzle[9][9], int solution[9][9], Difficulty difficulty);

private:
    bool fillHelper(int grid[9][9]);
    SudokuSolver solver;
};

#endif // SUDOKU_H
