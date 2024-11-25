#include <iostream> 
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <climits>
#include <utility>
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

// Node class to represent the state of the puzzle
class Node {
private:
    Node* parent; // Pointer to the parent node
    char puzzle[7]; // Current puzzle configuration
    int PositionOfEmptyTile; // Index of the empty tile ('K')
    int total_cost; // Total cost of reaching this state
    int depth_level; // Depth of the node in the search tree
    int last_move_cost; // Cost of the last move

    // Set of valid goal states for the puzzle
    unordered_set<string> solution_goals = {
        "WWWBBBK", "WWWBBKB", "WWWBKBB", "WWWKBBB", "WWKWBBB", "WKWWBBB", "KWWWBBB"
    };

public:
    // Default constructor
    Node() {}

    // Parameterized constructor
    Node(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int total_cost, int depth_level, int last_move_cost) {
        this->parent = parent;
        this->PositionOfEmptyTile = PositionOfEmptyTile;
        this->total_cost = total_cost;
        this->depth_level = depth_level;
        this->last_move_cost = last_move_cost;
        memcpy(this->puzzle, puzzle, sizeof(this->puzzle)); // Copy the puzzle state
    }

    // Create a new node by applying a move to the puzzle
    Node* newNode(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int depth_level, int last_move_cost) {
        char newPuzzle[7];
        memcpy(newPuzzle, puzzle, sizeof(newPuzzle)); // Copy current puzzle state
        swap(newPuzzle[PositionOfEmptyTile], newPuzzle[new_PositionOfEmptyTile]); // Swap the empty tile with the target tile
        return new Node(parent, newPuzzle, new_PositionOfEmptyTile, 0, depth_level + 1, last_move_cost);
    }

    // Print the current state of the puzzle
    void printState(const char puzzle[7]) {
        for (int i = 0; i < 7; i++) {
            cout << puzzle[i] << " | ";
        }
        cout << endl;
    }

    // Print the path from the root node to the current node
    void printPath(Node* root) {
        vector<Node*> path; // Store the nodes in the solution path
        int totalCost = 0;

        // Traverse from the current node to the root
        while (root != nullptr) {
            path.push_back(root);
            if (root->parent != nullptr) {
                totalCost += root->last_move_cost; // Accumulate move costs
            }
            root = root->parent;
        }

        reverse(path.begin(), path.end()); // Reverse the path to start from the root

        for (Node* node : path) {
            cout << "Puzzle State: ";
            for (int i = 0; i < 7; i++) cout << node->puzzle[i] << " | ";
            cout << endl;
            if (node->parent != nullptr) {
                cout << "MOVE COST: " << node->last_move_cost << endl;
                cout << "TOTAL DEPTH: " << node->depth_level << endl;
            }
            cout << endl;
        }
        // Print the total cost
        cout << "Total cost of the solution: " << totalCost << endl;
    }

    // Check if the current puzzle state is a goal state
    bool GoalCheck(const char puzzle[7]) {
        return solution_goals.find(string(puzzle, puzzle + 7)) != solution_goals.end();
    }

    // Perform Depth-First Search (DFS) with a depth limit
    bool DFS(char initialPuzzle[7], int emptyTilePos, int depthLimit, int& nodesExpanded) {
        stack<Node*> stack; // Stack for DFS
        vector<Node*> allocatedNodes; // Track allocated nodes to clean up later
        Node* root = new Node(nullptr, initialPuzzle, emptyTilePos, 0, 0, 0); // Create the root node
        stack.push(root);
        allocatedNodes.push_back(root);

        unordered_set<string> visited; // Track visited states
        Node* solution = nullptr;

        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();

            string currentState(current->puzzle, current->puzzle + 7);
            if (visited.find(currentState) != visited.end()) {
                continue; // Skip if the state was already visited
            }
            visited.insert(currentState);

            cout << "Current state at depth " << current->depth_level << ": ";
            printState(current->puzzle);

            if (GoalCheck(current->puzzle)) { // Check if the goal is reached
                solution = current;
                cout << "Solution found!\nPath to solution:\n";
                printPath(solution);
                cout << "Nodes expanded in DFS: " << nodesExpanded << endl;

                // Clean up allocated nodes
                for (Node* node : allocatedNodes) {
                    delete node;
                }
                return true; // Solution found
            }

            if (current->depth_level < depthLimit) { // Expand node if within depth limit
                vector<int> moves = {3, 2, 1, -1, -2, -3}; // Possible moves for the empty tile
                for (int move : moves) {
                    int newPos = current->PositionOfEmptyTile + move;
                    if (newPos >= 0 && newPos < 7) { // Ensure the move is valid
                        Node* child = newNode(current, current->puzzle, current->PositionOfEmptyTile, newPos, current->depth_level, abs(move));
                        stack.push(child);
                        allocatedNodes.push_back(child);
                    }
                }
                nodesExpanded++; // Increment nodes expanded
            }
        }

        // Clean up allocated nodes
        for (Node* node : allocatedNodes) {
            delete node;
        }
        return false; // No solution found
    }

    void IDS(char initialPuzzle[7], int emptyTilePos, int maxDepthLimit) {
        int totalNodesExpanded = 0; // Track total nodes expanded

        for (int depthLimit = 0; depthLimit <= maxDepthLimit; ++depthLimit) {
            cout << "Running DFS with depth limit: " << depthLimit << endl;

            char puzzleCopy[7];
            memcpy(puzzleCopy, initialPuzzle, sizeof(puzzleCopy)); // Copy the initial puzzle state
            int emptyTilePosCopy = emptyTilePos;

            int nodesExpandedInDFS = 0; // Nodes expanded in the current DFS
            if (DFS(puzzleCopy, emptyTilePosCopy, depthLimit, nodesExpandedInDFS)) {
                totalNodesExpanded += nodesExpandedInDFS;
                cout << "Total nodes expanded: " << totalNodesExpanded << endl;
                return; // Exit after finding the solution
            }

            totalNodesExpanded += nodesExpandedInDFS;
            cout << "Nodes expanded at depth " << depthLimit << ": " << nodesExpandedInDFS << endl;
            cout << "No solution found at depth limit " << depthLimit << "." << endl;
        }

        cout << "No solution found within the maximum depth limit.\nTotal nodes expanded: " << totalNodesExpanded << endl;
    }

};

int main() {
    // Initial puzzle state
    char puzzle[7] = {'B', 'B', 'B', 'W', 'W', 'W', 'K'};
    int emptyTilePosition = 6; // Position of the empty tile
    Node solver;
    solver.IDS(puzzle, emptyTilePosition, 40); // Perform IDS with a maximum depth of 40
    return 0;
}
