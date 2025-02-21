#include "include.hpp"
#include <iostream>
#include <string>
using namespace std;
int main() {
  int row = 9;
  int col = 9;
  int mine_sum = 10;
  cout<<"Hello"<<endl;
  
  
  Grid grid(row, col);
  Game game(false, grid, "");
  grid.initialGrid(mine_sum);
  game.startGame(row, col, mine_sum);
  int x = 0;
  int y = 0;
  
  game.displayGame();
  while (!game.getGameover()) {
    
    char deal = 'a';
    while(deal != 'r' ) {
      if (deal == 'f') {
        break;
      }
      cout << "r to reveal , f to flag or cancel flag" << endl;
      cin >> deal;
    }
    cout << "enter x and y" << endl;
    cin >> x >> y;

    game.gameProgress(y, x,deal);
    game.displayGame();
  }
  cout<<game.getResult();
  return 0;
}