#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>
using namespace std;

unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();

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

bool backtrack_uniq(int puzzle[9][9], int row, int col, int& solutionCount) {
    if (row == 8 && col == 9) {
        solutionCount++;
        return solutionCount == 1;
    }
    if (col == 9) {
        row++;
        col = 0;
    }
    if (puzzle[row][col] > 0) {
        return backtrack_uniq(puzzle, row, col + 1, solutionCount);
    }
    for (int i = 1; i <= 9; i++) {
        if (valid(puzzle, row, col, i)) {
            puzzle[row][col] = i;
            if (!backtrack_uniq(puzzle, row, col + 1, solutionCount)) {
                return false;
            }
            puzzle[row][col] = 0;
        }
    }
    return true;
}

bool verify(int puzzle[9][9]) { // Check grid to determine whether solutions are unique
    int solutionCount = 0;
    backtrack_uniq(puzzle, 0, 0, solutionCount);
    return solutionCount == 1;
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

void puzzlegen(int sudoku[9][9], int totalBlanks) {
    int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    default_random_engine engine(seed);
    for(int i = 0; i < 3; i++) {
        shuffle(values, values + 9, engine);
        for(int j = 0; j < 9; j++) {
            sudoku[i*3 + j%3][i*3 + j/3] = values[j];
        }
    }
    backtrack(sudoku, 0, 0);
    int indices[81];
    vector<int> removed;
    for(int i = 0; i < 81; i++) indices[i] = i+1;
    int blanks = 0;
    int index = 0;
    int solutionCount = 0;
    shuffle(indices, indices + 81, engine);
    while(blanks < totalBlanks && index < 81) {
        solutionCount = 0;
        int x = indices[index];
        sudoku[x/9][x%9] = 0;
        for(int i = 0; i < removed.size(); i++) sudoku[removed[i]/9][removed[i]%9] = 0;
        backtrack_uniq(sudoku, 0, 0, solutionCount);
        if(solutionCount == 1) {
            removed.push_back(x);
            blanks++;
        }
        index++;
    }
    for(int i = 0; i < removed.size(); i++) sudoku[removed[i]/9][removed[i]%9] = 0;
    cout << "Unique puzzle generated.";
    if(index == 81) cout << " Only " << removed.size() << " blank spaces are possible for a unique solution.";
    cout << endl;
}

int main(int argc, char* argv[]) {
    int totalBlanks = 9;
    if(argc > 1) totalBlanks = atoi(argv[1]);
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
    puzzlegen(sudoku, totalBlanks);
    printpuzzle(sudoku);
    int sudoku2[9][9];
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) sudoku2[i][j] = sudoku[i][j];
    }
    chrono::_V2::system_clock::time_point start;
    chrono::_V2::system_clock::time_point stop;
    bool solved;
    chrono::duration<double> secs;

    if(argc == 3 && strcmp(argv[2], "backtrack") == 0) {
        printf("Skipped brute force solution\n");
    } else {
        start = chrono::high_resolution_clock::now();
        solved = bruteforce(sudoku);
        stop = chrono::high_resolution_clock::now();
        secs = stop-start;
        if(!solved) {
            printf("Puzzle appears to be unsolvable via brute force after %lf seconds\n", secs.count());
        } else {
            printf("Puzzle solved with brute force in %lf seconds\n", secs.count());
            printpuzzle(sudoku);
        }
    }

    start = chrono::high_resolution_clock::now();
    solved = backtrack(sudoku2, 0, 0);
    stop = chrono::high_resolution_clock::now();
    secs = stop-start;
    if(!solved) {
        printf("Puzzle appears to be unsolvable via backtracking after %lf seconds\n", secs.count());
    } else {
        printf("Puzzle solved with backtracking in %lf seconds\n", secs.count());
        printpuzzle(sudoku);
    }
}