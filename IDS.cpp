#include <iostream> 
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <climits>
#include <utility>
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

class Node {
private:
    Node* parent; // Pointer to the parent node in the search tree
    char puzzle[7]; // The current puzzle configuration (array of characters)
    int PositionOfEmptyTile; // Index of the empty tile ('K') in the puzzle
    int depth_level; // Depth level of the node in the search tree
    int last_move_cost; // Cost of the last move to reach this node

//Set of all possible goal states for the puzzle
    unordered_set<string> solution_goals = {
        "WWWBBBK", "WWWBBKB", "WWWBKBB", "WWWKBBB", "WWKWBBB", "WKWWBBB", "KWWWBBB"
    };

public:
//Constructor without parameters
    Node() {}
// constructor with parameters
    Node(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int depth_level, int last_move_cost) {
        this->parent = parent;
        this->PositionOfEmptyTile = PositionOfEmptyTile;
        this->depth_level = depth_level;
        this->last_move_cost = last_move_cost;
        memcpy(this->puzzle, puzzle, sizeof(this->puzzle));
    }
//This function creates a new node by applying a move to the puzzle
    Node* newNode(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int depth_level, int last_move_cost) {
        char newPuzzle[7];
        memcpy(newPuzzle, puzzle, sizeof(newPuzzle));
        swap(newPuzzle[PositionOfEmptyTile], newPuzzle[new_PositionOfEmptyTile]); // Swap the empty tile with the target tile
        return new Node(parent, newPuzzle, new_PositionOfEmptyTile, depth_level + 1, last_move_cost); //Create and return the new node
    }
//This function prints the path from the root node to the current node
    void printPath(Node* root) {
        vector<Node*> path; //Vector to store the nodes along the solution path
        int totalCost = 0; //Variable to track the total cost of the solution

//Traverses back from the current node to the root nod
        while (root != nullptr) {
            path.push_back(root); //It puts the current node to the end of path vector.
            if (root->parent != nullptr) {
                totalCost += root->last_move_cost; //Adds the costs
            }
            root = root->parent; // Move to the parent node
        }
//Reverse the path vector to display the path in the correct order (root to solution)
        reverse(path.begin(), path.end());

// Print each node in the solution path
        for (Node* node : path) {
            cout << "Puzzle State: ";
            for (int i = 0; i < 7; i++) cout << node->puzzle[i] << " | ";
            cout << endl;
//Prints additional info if the node is not the root
            if (node->parent != nullptr) {
                cout << "MOVE COST: " << node->last_move_cost << endl;
                cout << "TOTAL DEPTH: " << node->depth_level << endl;
            }
            cout << endl;
        }
//Prints the total cost of the solution
        cout << "Total cost of the solution: " << totalCost << endl;
    }
//This function checks if the current state is one of the solutions
    bool GoalCheck(const char puzzle[7]) {
//Converts the puzzle array to a string and check if it exists in the solution_goals set
        return solution_goals.find(string(puzzle, puzzle + 7)) != solution_goals.end();
    }
//The DFS algorithm function
    bool DFS(char initialPuzzle[7], int emptyTilePos, int depthLimit, int& totalNodesCreated) {
        stack<Node*> stack; // Stack to store nodes for traversal
        vector<Node*> allocatedNodes; // Keeps track of all dynamically allocated nodes to ensure cleanup
// Creates the root node
        Node* root = new Node(nullptr, initialPuzzle, emptyTilePos, 0, 0);
        stack.push(root);
        allocatedNodes.push_back(root);
        totalNodesCreated++; //To count the root node

// A set to keep track of visited puzzle states to avoid revisiting them
        unordered_set<string> visited;
        Node* solution = nullptr;

// Main DFS loop: continues until the stack is empty
        while (!stack.empty()) {
            Node* current = stack.top(); // Get the node at the top of the stack
            stack.pop();

            string currentState(current->puzzle, current->puzzle + 7);

// Skip this state if it has already been visited
            if (visited.find(currentState) != visited.end()) {
                continue;
            }
            visited.insert(currentState);

// Checks if the current state is the goal state and print sthe path to the solution
            if (GoalCheck(current->puzzle)) {
                solution = current;
                cout << "Solution found!\nPath to solution:\n";
                printPath(solution);

// Clean up all dynamically allocated nodes
                for (Node* node : allocatedNodes) {
                    delete node;
                }
                return true;
            }

//Expands the current node if it's within the depth limit
            if (current->depth_level < depthLimit) {
                vector<int> moves = {3, 2, 1, -1, -2, -3}; // Possible moves for the empty tile
                for (int move : moves) {
                    int newPos = current->PositionOfEmptyTile + move;
 // Checks if the new position is valid (within bounds)
                    if (newPos >= 0 && newPos < 7) {
 // Creates a new node representing the new puzzle state after the move
                        Node* child = newNode(current, current->puzzle, current->PositionOfEmptyTile, newPos, current->depth_level, abs(move));
                        stack.push(child);
                        allocatedNodes.push_back(child);
                    totalNodesCreated++; // Increases the count for each new node    
                    }
                }
            }
        }
// Clean up all dynamically allocated nodes if no solution is found
        for (Node* node : allocatedNodes) {
            delete node;
        }
        return false;
    }

//The IDS algorithm function
    void IDS(char initialPuzzle[7], int emptyTilePos, int maxDepthLimit) {
        int totalNodesCreated = 0;

// Loop through increasing depth limits from 0 to the maximum depth limit
        for (int depthLimit = 0; depthLimit <= maxDepthLimit; ++depthLimit) {

            char puzzleCopy[7];
            memcpy(puzzleCopy, initialPuzzle, sizeof(puzzleCopy));  // Creates a copy of the initial puzzle state
            int emptyTilePosCopy = emptyTilePos; // Copies the position of the empty tile (to ensure the DFS always runs from the same initial state)

// Performs Depth-First Search (DFS) with the current depth limit            
            if (DFS(puzzleCopy, emptyTilePosCopy, depthLimit, totalNodesCreated)) {

                cout << "Total nodes created: " << totalNodesCreated << endl;
                return; // Exit the function as the solution has been found
            }
        }
 // If no solution is found after trying all depth limits, print the result
        cout << "No solution found within the maximum depth limit.\nTotal nodes created: " << totalNodesCreated << endl;
    }
};

int main() {
// Initial puzzle state
    char puzzle[7] = {'B', 'B', 'B', 'W', 'W', 'W', 'K'};
    int emptyTilePosition = 6; // Position of the empty tile ('K')
//creates an object of the Node class in order to use its functions.
    Node solver;
 // Performs Iterative Deepening Search (IDS) with a maximum depth limit of 40
    solver.IDS(puzzle, emptyTilePosition, 40);
    return 0;
}
