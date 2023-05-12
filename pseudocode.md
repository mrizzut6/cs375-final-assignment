### Full-grid backtracking

Input: 6x6 Sudoku grid with blanks initialized to 0
Output: Boolean which returns true if successfully solved

backtrackSolve(grid S) {
    int row; int column;

    // If there are no blank spaces
    return true;

    for (i from 1 to 9) {
        // If i isn't elsewhere in row or column
        if (backtrackSolve(S)) return true;
        // If recursion doesn't return true
        grid[row][col] = 0; // Value not known, backtrack
    }

    return false;
}

### Minigrid backtracking

Input: 6x6 Sudoku grid with blanks initialized to 0
Output: Boolean which returns true if successfully solved

miniBTSolve(grid S) {
    for (minigrid 1 to 9) {
        generatePermutations();
    }
    minigridSequence = mg1, mg2, ..., mg9
    return miniBTSolve(grid S, mg1, mg2);
}

miniBTSolve(grid S, minigrid1, minigrid2) {
    if (minigrid2 is null) return true;

    for (all permutations of minigrid1) {
        for (all permutations of minigrid2) {
            if (!miniBTSolve(S, minigrid2, minigrid2.next)) break;
        }
    }

    return false;
}