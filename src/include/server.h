#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to use global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows;         // The count of rows of the game map. You MUST NOT modify its name.
int columns;      // The count of columns of the game map. You MUST NOT modify its name.
int total_mines;  // The count of mines of the game map. You MUST NOT modify its name. You should initialize this
                  // variable in function InitMap. It will be used in the advanced task.
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.

// Additional global variables
char map[35][35];        // Map storing mine positions ('X' for mine, '.' for non-mine)
int state[35][35];       // 0=unvisited, 1=visited, 2=marked
int mine_count[35][35];  // Count of adjacent mines for each grid

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map
 * would be initialized, with all the blocks unvisited.
 */
void InitMap() {
  std::cin >> rows >> columns;
  total_mines = 0;
  game_state = 0;

  // Read the map
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      std::cin >> map[i][j];
      state[i][j] = 0;  // All unvisited
      if (map[i][j] == 'X') {
        total_mines++;
      }
    }
  }

  // Calculate mine counts for each grid
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      mine_count[i][j] = 0;
      if (map[i][j] == '.') {
        // Count adjacent mines
        for (int di = -1; di <= 1; di++) {
          for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && map[ni][nj] == 'X') {
              mine_count[i][j]++;
            }
          }
        }
      }
    }
  }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void VisitBlock(int r, int c) {
  // Check bounds
  if (r < 0 || r >= rows || c < 0 || c >= columns) {
    return;
  }

  // If already visited or marked, no effect
  if (state[r][c] == 1 || state[r][c] == 2) {
    return;
  }

  // Mark as visited
  state[r][c] = 1;

  // If it's a mine, game over
  if (map[r][c] == 'X') {
    game_state = -1;
    return;
  }

  // If mine count is 0, recursively visit all adjacent grids
  if (mine_count[r][c] == 0) {
    for (int di = -1; di <= 1; di++) {
      for (int dj = -1; dj <= 1; dj++) {
        if (di == 0 && dj == 0) continue;
        int nr = r + di, nc = c + dj;
        if (nr >= 0 && nr < rows && nc >= 0 && nc < columns) {
          VisitBlock(nr, nc);
        }
      }
    }
  }

  // Check win condition: all non-mine grids visited
  if (game_state == 0) {
    int visited_count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (map[i][j] == '.' && state[i][j] == 1) {
          visited_count++;
        }
      }
    }
    if (visited_count == rows * columns - total_mines) {
      game_state = 1;
    }
  }
}

/**
 * @brief The definition of function MarkMine(int, int)
 *
 * @details This function is designed to mark a mine in the game map.
 * If the block being marked is a mine, show it as "@".
 * If the block being marked isn't a mine, END THE GAME immediately. (NOTE: This is not the same rule as the real
 * game) And you don't need to
 *
 * For example, if we use the same map as before, and the current state is:
 *     1?1
 *     ???
 *     ???
 * If you call MarkMine(0, 1), you marked the right mine. Then the resulting game map is:
 *     1@1
 *     ???
 *     ???
 * If you call MarkMine(1, 0), you marked the wrong mine(There's no mine in grid (1, 0)).
 * The game_state would be -1 and game ends immediately. The game map would be:
 *     1?1
 *     X??
 *     ???
 * This is different from the Minesweeper you've played. You should beware of that.
 *
 * @param r The row coordinate (0-based) of the block to be marked.
 * @param c The column coordinate (0-based) of the block to be marked.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void MarkMine(int r, int c) {
  // Check bounds
  if (r < 0 || r >= rows || c < 0 || c >= columns) {
    return;
  }

  // If already visited or marked, no effect
  if (state[r][c] == 1 || state[r][c] == 2) {
    return;
  }

  // Mark the grid
  state[r][c] = 2;

  // If it's not a mine, game over immediately
  if (map[r][c] == '.') {
    game_state = -1;
    return;
  }

  // Check win condition: all non-mine grids visited
  if (game_state == 0) {
    int visited_count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (map[i][j] == '.' && state[i][j] == 1) {
          visited_count++;
        }
      }
    }
    if (visited_count == rows * columns - total_mines) {
      game_state = 1;
    }
  }
}

/**
 * @brief The definition of function AutoExplore(int, int)
 *
 * @details This function is designed to auto-visit adjacent blocks of a certain block.
 * See README.md for more information
 *
 * For example, if we use the same map as before, and the current map is:
 *     ?@?
 *     ?2?
 *     ??@
 * Then auto explore is available only for block (1, 1). If you call AutoExplore(1, 1), the resulting map will be:
 *     1@1
 *     122
 *     01@
 * And the game ends (and player wins).
 */
void AutoExplore(int r, int c) {
  // Check bounds
  if (r < 0 || r >= rows || c < 0 || c >= columns) {
    return;
  }

  // Can only target visited non-mine grids
  if (state[r][c] != 1 || map[r][c] == 'X') {
    return;
  }

  // Count marked mines around the target
  int marked_mines = 0;
  for (int di = -1; di <= 1; di++) {
    for (int dj = -1; dj <= 1; dj++) {
      if (di == 0 && dj == 0) continue;
      int nr = r + di, nc = c + dj;
      if (nr >= 0 && nr < rows && nc >= 0 && nc < columns && state[nr][nc] == 2) {
        marked_mines++;
      }
    }
  }

  // If marked mines equals mine count, visit all unmarked unvisited grids
  if (marked_mines == mine_count[r][c]) {
    for (int di = -1; di <= 1; di++) {
      for (int dj = -1; dj <= 1; dj++) {
        if (di == 0 && dj == 0) continue;
        int nr = r + di, nc = c + dj;
        if (nr >= 0 && nr < rows && nc >= 0 && nc < columns && state[nr][nc] == 0) {
          VisitBlock(nr, nc);
          if (game_state != 0) return;  // Stop if game ends
        }
      }
    }
  }
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and marked_mine_count,
 * representing the number of blocks visited and the number of marked mines taken respectively.
 *
 * @note If the player wins, we consider that ALL mines are correctly marked.
 */
void ExitGame() {
  if (game_state == 1) {
    std::cout << "YOU WIN!" << std::endl;
  } else {
    std::cout << "GAME OVER!" << std::endl;
  }

  // Count visited non-mine grids
  int visit_count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (map[i][j] == '.' && state[i][j] == 1) {
        visit_count++;
      }
    }
  }

  // Count marked mines
  int marked_mine_count = 0;
  if (game_state == 1) {
    // If win, all mines are correctly marked
    marked_mine_count = total_mines;
  } else {
    // If lose, count correctly marked mines
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (map[i][j] == 'X' && state[i][j] == 2) {
          marked_mine_count++;
        }
      }
    }
  }

  std::cout << visit_count << " " << marked_mine_count << std::endl;
  exit(0);  // Exit the game immediately
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (game_state == 1 && map[i][j] == 'X') {
        // Victory: show all mines as @
        std::cout << '@';
      } else if (state[i][j] == 0) {
        // Unvisited
        std::cout << '?';
      } else if (state[i][j] == 2) {
        // Marked
        if (map[i][j] == 'X') {
          // Marked mine
          std::cout << '@';
        } else {
          // Marked non-mine (causes failure)
          std::cout << 'X';
        }
      } else {
        // Visited
        if (map[i][j] == 'X') {
          // Visited mine
          std::cout << 'X';
        } else {
          // Visited non-mine, show mine count
          std::cout << mine_count[i][j];
        }
      }
    }
    std::cout << std::endl;
  }
}

#endif
