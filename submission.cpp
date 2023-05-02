#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

void printpuzzle(int puzzle[9][9]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cout << puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

bool solved(int puzzle[9][9]) {
    int primes[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for(int i = 0; i < 9; i++) {
        long prodrow = 1;
        long prodcol = 1;
        long prodsq = 1;
        for(int j = 0; j < 9; j++) {
            prodrow *= primes[puzzle[i][j]-1];
            prodcol *= primes[puzzle[j][i]-1];
            prodsq *= primes[puzzle[(3*i)%9 + (j%3)][(i/3)*3 + (j/3)]-1];
        }
        if(prodrow != 223092870 || prodcol != 223092870 || prodsq != 223092870) return false;
    }
    return true;
}

bool valid(int puzzle[9][9], int x, int y, int val) {
    for(int i = 0; i < 9; i++) {
        if(val == puzzle[i][y] && i != x) return false;
        if(val == puzzle[x][i] && i != y) return false;
        if(val == puzzle[(x/3)*3+i/3][y/3*3+i%3] && ((x/3)*3+i/3 != x || (y/3)*3+i%3 != y)) return false;
    }
    return true;
}

bool backtrack(int puzzle[9][9], int row, int col) {
    if(row == 8 && col == 9) return true;
    if(col == 9) {
        row++;
        col = 0;
    }
    if(puzzle[row][col] > 0) return backtrack(puzzle, row, col+1);
    for(int i = 1; i <= 9; i++) {
        if(valid(puzzle, row, col, i)) {
            puzzle[row][col] = i;
            if(backtrack(puzzle, row, col+1)) return true;
            else puzzle[row][col] = 0;
        }
    }
    return false;
}

bool bruteforce(int puzzle[9][9]) {
    int spaces[81];
    int index = 0;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(puzzle[i][j] < 1 || puzzle[i][j] > 9) {
                spaces[index] = (i*9 + j);
                index++;
            }
        }
    }
    for(int i = 0; i < index; i++) puzzle[spaces[i]/9][spaces[i] % 9] = 1;
    while(!solved(puzzle)) {
        puzzle[spaces[0]/9][spaces[0] % 9] += 1;
        int place = 0;
        while(puzzle[spaces[place]/9][spaces[place] % 9] > 9) {
            puzzle[spaces[place]/9][spaces[place] % 9] -= 9;
            place++;
            puzzle[spaces[place]/9][spaces[place] % 9] += 1;
            if(place >= index) return false;
        }
    }
    return true;
}

void puzzlegen(int sudoku[9][9]) {
    int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(int i = 0; i < 3; i++) {
        shuffle(values, values + 9, default_random_engine());
        for(int j = 0; j < 9; j++) {
            sudoku[i*3 + j%3][i*3 + j/3] = values[j];
        }
    }
    backtrack(sudoku, 0, 3);
    for(int i = 0; i < 10; i++) {
        int rx = (rand() % 9);
        int ry = (rand() % 9);
        sudoku[rx][ry] = 0;
    }
    // TODO: Check if sudoku has a unique solution
}

int main() {
    int sudoku[9][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    puzzlegen(sudoku);
    printpuzzle(sudoku);

    auto start = chrono::high_resolution_clock::now();
    bool solved = bruteforce(sudoku);
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> secs = stop-start;
    if(solved) {
        printf("Puzzle appears to be unsolvable\n");
    }
    printf("Puzzle solved in %lf seconds\n", secs.count());
    printpuzzle(sudoku);
}