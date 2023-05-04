#include <iostream>
#include <vector>
#include <deque>

class treeNode {
    private:
        int v;
        std::vector<treeNode> children;
    public:
        treeNode() {
            
        }

        treeNode(const int v) {
            this->v = v;
        }

        int getValue() {
            return this->v;
        }

        void setValue(const int t) {
            this->v = v;
        }

        void addNode(const int v) {
            this->children.push_back(treeNode(v));
        }

        void addUniqLeaves(std::vector<int> nodes) { // add unique values only to leaf nodes
            if (this->children.empty()) { // if no children
                for (std::vector<treeNode>::iterator it = this->children.begin(); it != this->children.end(); it++) {
                    for (std::vector<int>::iterator it_nodes = nodes.begin(); it_nodes != nodes.end(); it_nodes++) {
                        if (it->v != *it_nodes) it->addNode(*it_nodes);
                    } 
                }
                return; 
            }

            // ensure that no node is added with a value equal to that of a node higher in the subtree
            std::vector<int>::iterator check = find(nodes.begin(), nodes.end(), this-v);
            if (nodes.end() != check) nodes.erase(check);

            for (std::vector<treeNode>::iterator it = this->children.begin(); it != this->children.end(); it++) {
                it->addUniqLeaves(nodes);
            }

        }

        std::vector<treeNode> getChildren() {
            return this->children;
        }

        std::vector< std::deque<int> > getLeafPaths(treeNode root) {
            std::vector< std::deque<int> > paths;
            return getLeafPaths(root, paths);
        }

        std::vector< std::deque<int> > getLeafPaths(treeNode root, std::vector< std::deque<int> > paths) {
            
        }
};

bool checkSpace(int puzzle[9][9], int row, int col, int val) {
    for (int i = 0; i < 9; i++) {
        // if value is found in row or column of space, abort
        if ((puzzle[row][i] == val) || (puzzle[i][col] == val)) return true;
    }

    return false;  // otherwise, value could be placed here
}

void createTree(int puzzle[9][9], int row, int col, treeNode trees[9], int gridNum) {
    for (int i = 0; i < 3; i++) { // for row i
        for (int j = 0; j < 3; j++) { // for col j
            if (!puzzle[row+i][col+j]) { // empty cell
                std::vector<int> candidates;
                for (int k = 0; k < 9; k++) { // for value k 
                    if (!checkSpace(puzzle, row+i, col+j, k)) candidates.push_back(k);
                }
                // add the list of possible nums for this blank cell to the tree
                trees[gridNum].addUniqLeaves(candidates);
            }
        }
    }
}

bool backtrack(int puzzle[9][9]) {
    treeNode trees[9];

    // Step 1: Compute all currently valid permutations
    // Yes, it's weird, sorry

    int gridNum = 0;
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            createTree(puzzle, i, j, trees, gridNum);
            gridNum++;
        }
    }

    // Step 2: Try and form a sequence of permutations
    
}