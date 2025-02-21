#ifndef INCLUDE_HPP
#define INCLUDE_HPP
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
enum CellState {
  Covered,
  Released,
  Flagged,
};
class Cell {
private:
  CellState state_;
  int mine_count_;
  bool is_mine_;

public:
  Cell() : state_(Covered), mine_count_(0), is_mine_(false) {}
  Cell(CellState state) : state_(state), mine_count_(0), is_mine_(false) {}
  Cell(CellState state, int mine_count, bool is_mine)
      : state_(state), mine_count_(mine_count), is_mine_(is_mine) {}
  void setMine(bool is_mine) { is_mine_ = is_mine; }
  void setMineCount(int mine_count) { mine_count_ = mine_count; }
  void setState(CellState state) { state_ = state; }
  void flag() {
    if (state_ == Covered) {
      state_ = Flagged;
    }
  }
  void unflag() {
    if (state_ == Flagged) {
      state_ = Covered;
    }
  }
  void reveal() { state_ = Released; }
  bool isMineCell() const { return is_mine_; }
  int getMineCount() const { return mine_count_; }
  CellState getState() const { return state_; }
  void display_cell() const {
    if (state_ == Covered) {
      std::cout << "? ";
    } else if (state_ == Flagged) {
      std::cout << "F ";
    } else if (state_ == Released) {
      if (is_mine_) {
        std::cout << "M ";
      } else
        std::cout << std::to_string(mine_count_) << " ";
    }
  }
  ~Cell() = default;
};
class Grid {
private:
  int row_;
  int col_;
  std::vector<std::vector<Cell>> cells;

public:
  Grid(int row, int col) : row_(row), col_(col) {
    cells.resize(row, std::vector<Cell>(col));
    row_ = row;
    col_ = col;
  }
  ~Grid() = default;
  int getMineCount(int x, int y) {
    int count = 0;
    for (int i = std::max(0, x - 1); i < std::min(x + 2, row_); ++i) {
      for (int j = std::max(0, y - 1); j < std::min(y + 2, col_); ++j) {
        if (cells[i][j].isMineCell()) {
          count++;
        }
      }
    }
    return count;
  }
  void initialGrid(int mine_sum) {
   
    std::srand(static_cast<unsigned>(time(0)));
    int count = 0;

    if (mine_sum >= row_ * col_) {
      std::cerr << "Error: Too many mines for the grid size!" << std::endl;
      return;
    }

    // 放置地雷
    while (count < mine_sum) {
      int x = rand() % row_;
      int y = rand() % col_;
      if (!cells[x][y].isMineCell()) {
        cells[x][y].setMine(true);
        count++;
      }
    }

    // 计算每个单元格周围的地雷数量
    for (int i = 0; i < row_; ++i) {
      for (int j = 0; j < col_; ++j) {
        if (!cells[i][j].isMineCell()) {
          cells[i][j].setMineCount(getMineCount(i, j));
        }
      }
    }

    
  }
  Cell &getCell(int x, int y) { return cells[x][y]; }
  void lose() {
    for (auto& vectors : cells) {
      for (auto& it : vectors) {
        if (it.isMineCell()) {
          it.reveal();
        }
        
      }
    }
  }
  bool revealCell(int x, int y) {
    Cell &cell = cells[x][y];

    if (cell.getState() == Covered) {
      cell.reveal();
      if (cell.isMineCell()) {
        lose();
        return true;
      }
      else if (cell.getMineCount() == 0) {

        for (int i = std::max(0, x - 1); i < std::min(x + 2, row_); ++i) {
          for (int j = std::max(y - 1, 0); j < std::min(y + 2, col_); ++j) {
            revealCell(i, j);
          }
        }
      }
    }
    return false;
  }
  void flagCell(int x, int y) {
    Cell &cell = cells[x][y];
    if (cell.getState() == Covered) {
      cell.flag();
    } else if (cell.getState() == Flagged) {
      cell.unflag();
    }
  }
  bool checkWin() {
    for (const auto& vectors : cells) {
      for(const auto& it : vectors) {
        if (!it.isMineCell()&& it.getState() != Released) {
          return false;
        }
      }
    }
    return true;
  }
  bool checkLose() {
    for (const auto& vectors : cells) {
      for(const auto& it : vectors) {
        if (it.isMineCell()&& it.getState() == Released) {
          return true;
        }
      }
    }
    return false;
  }
  void displayGrid() {
    for (const auto &vectors : cells) {
      for (const auto &it : vectors) {
        it.display_cell();
      }
      std::cout << std::endl;
    }
  }
};
class Game {
private:
  bool gameover_;
  Grid grid_;
  std::string result_;

public:
  Game(bool gameover, Grid grid, std::string result)
      : gameover_(gameover), grid_(grid), result_(result) {}
  ~Game() = default;
  void startGame(int row, int col, int mine_sum) {
    
    grid_ = Grid(row, col);
    grid_.initialGrid(mine_sum);
    gameover_ = false;
    result_ = "";
   
  }
  bool getGameover() {
    return gameover_;
  }
  void setGameover(bool gameover) {
    gameover_ = gameover;
  }
  std::string getResult() {
    return result_;
  }
  void setResult(std::string result) {
    result_ = result;
  }
  void gameProgress(int x, int y, char deal) {
    Cell &cell = grid_.getCell(x, y);
    
    if (deal == 'r') {
      
      if (grid_.revealCell(x, y)) {
        gameover_ = true;
        result_ = "You lose!";
      }
      else if (grid_.checkWin()) {
        gameover_ = grid_.checkWin();
        result_ = "You win!";
      }
    } else if (deal == 'f') {
      grid_.flagCell(x, y);
    } else {
      std::cout << "Please enter r or f" << std::endl;
      return;
    }
    
  }

  void displayGame() { grid_.displayGrid(); }
};
#endif