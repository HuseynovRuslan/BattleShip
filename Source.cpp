#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int grid_size = 10;
const int ship_count = 10;
const char water = '~';
const char ship = 'S';
const char miss = 'M';
const char hit = 'X';


const string reset = "\033[0m";
const string blue = "\033[34m";
const string red = "\033[31m";
const string green = "\033[32m";
const string yellow = "\033[33m";
const string cyan = "\033[36m";


int ship_sizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

void clear_screen() {
    system("cls");
}

void print_grids(char player_grid[grid_size][grid_size], char computer_grid[grid_size][grid_size]) {
    cout << "Your grid:               Enemy's grid:" << endl;
    cout << "  0 1 2 3 4 5 6 7 8 9      0 1 2 3 4 5 6 7 8 9" << endl;

    for (int i = 0; i < grid_size; ++i) {
        cout << i << " ";
        for (int j = 0; j < grid_size; ++j) {
            if (player_grid[i][j] == water) {
                cout << blue << water << " " << reset;
            }
            else if (player_grid[i][j] == ship) {
                cout << green << ship << " " << reset;
            }
            else if (player_grid[i][j] == miss) {
                cout << yellow << miss << " " << reset;
            }
            else if (player_grid[i][j] == hit) {
                cout << red << hit << " " << reset;
            }
        }

        cout << "   " << i << " ";
        for (int j = 0; j < grid_size; ++j) {
            if (computer_grid[i][j] == water) {
                cout << blue << water << " " << reset;
            }
            else if (computer_grid[i][j] == miss) {
                cout << yellow << miss << " " << reset;
            }
            else if (computer_grid[i][j] == hit) {
                cout << red << hit << " " << reset;
            }
            else {
                cout << blue << water << " " << reset;
            }
        }
        cout << endl;
    }
}

bool is_hit(char grid[grid_size][grid_size], int row, int col) {
    return grid[row][col] == ship;
}

bool is_miss(char grid[grid_size][grid_size], int row, int col) {
    return grid[row][col] == miss || grid[row][col] == hit;
}

void player_move(char enemy_grid[grid_size][grid_size], int& remaining_ships, bool& player_turn) {
    int row, col;
    cout << cyan << "Enter your target (row and column): " << reset;
    cin >> row >> col;

    if (row < 0 || row >= grid_size || col < 0 || col >= grid_size || is_miss(enemy_grid, row, col)) {
        cout << red << "Invalid move! Try again." << reset << endl;
        return;
    }

    if (is_hit(enemy_grid, row, col)) {
        cout << green << "You hit a ship!" << reset << endl;
        enemy_grid[row][col] = hit;
        --remaining_ships;
        player_turn = true;
    }
    else {
        cout << yellow << "You missed!" << reset << endl;
        enemy_grid[row][col] = miss;
        player_turn = false;
    }
}

void computer_move(char player_grid[grid_size][grid_size], int& remaining_ships, bool& computer_turn) {
    int row, col;
    do {
        row = rand() % grid_size;
        col = rand() % grid_size;
    } while (is_miss(player_grid, row, col));

    cout << cyan << "Computer fires at (" << row << ", " << col << ")" << reset << endl;

    if (is_hit(player_grid, row, col)) {
        cout << red << "Computer hit your ship!" << reset << endl;
        player_grid[row][col] = hit;
        --remaining_ships;
        computer_turn = true;
    }
    else {
        cout << yellow << "Computer missed!" << reset << endl;
        player_grid[row][col] = miss;
        computer_turn = false;
    }
}

bool can_place_ship_with_gap(char grid[grid_size][grid_size], int row, int col, int size, bool horizontal) {
    int row_start = max(0, row - 1);
    int col_start = max(0, col - 1);

    int row_end;
    if (horizontal) {
        row_end = min(grid_size - 1, row + 0 + 1);
    }
    else {
        row_end = min(grid_size - 1, row + size + 1);
    }

    int col_end;
    if (horizontal) {
        col_end = min(grid_size - 1, col + size + 1);
    }
    else {
        col_end = min(grid_size - 1, col + 0 + 1);
    }

    for (int i = row_start; i <= row_end; ++i) {
        for (int j = col_start; j <= col_end; ++j) {
            if (grid[i][j] != water) return false;
        }
    }

    return true;
}

void place_ship_with_gap(char grid[grid_size][grid_size], int size) {
    bool placed = false;
    while (!placed) {
        int row = rand() % grid_size;
        int col = rand() % grid_size;
        bool horizontal = rand() % 2 == 0;

        if (can_place_ship_with_gap(grid, row, col, size, horizontal)) {
            if (horizontal) {
                for (int i = 0; i < size; ++i) {
                    grid[row][col + i] = ship;
                }
            }
            else {
                for (int i = 0; i < size; ++i) {
                    grid[row + i][col] = ship;
                }
            }
            placed = true;
        }
    }
}

void initialize_grid(char grid[grid_size][grid_size]) {
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            grid[i][j] = water;
        }
    }
}

void place_ships(char grid[grid_size][grid_size], int ship_sizes[], int ship_count) {
    for (int i = 0; i < ship_count; ++i) {
        place_ship_with_gap(grid, ship_sizes[i]);
    }
}

int main() {
    srand(time(0));

    char player_grid[grid_size][grid_size];
    char computer_grid[grid_size][grid_size];

    initialize_grid(player_grid);
    initialize_grid(computer_grid);

    place_ships(player_grid, ship_sizes, ship_count);
    place_ships(computer_grid, ship_sizes, ship_count);

    int player_ships = ship_count;
    int computer_ships = ship_count;

    bool player_turn = true;
    bool computer_turn = false;

    while (player_ships > 0 && computer_ships > 0) {
        clear_screen();

        print_grids(player_grid, computer_grid);

        if (player_turn && !computer_turn) {
            player_move(computer_grid, computer_ships, player_turn);
            if (computer_ships == 0) {
                cout << green << "You win!" << reset << endl;
                break;
            }
        }

        if (!player_turn || computer_turn) {
            computer_move(player_grid, player_ships, computer_turn);
            if (player_ships == 0) {
                cout << red << "Computer wins!" << reset << endl;
                break;
            }

            if (!computer_turn) {
                player_turn = true;
            }
        }
    }

    return 0;
}