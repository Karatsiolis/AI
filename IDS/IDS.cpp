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

class Node {
private:
    Node* parent;
    char puzzle[7];
    int PositionOfEmptyTile;
    int total_cost;
    int depth_level;
    int last_move_cost;

    unordered_set<string> solution_goals = {
        "WWWBBBK", "WWWBBKB", "WWWBKBB", "WWWKBBB", "WWKWBBB", "WKWWBBB", "KWWWBBB"
    };

public:
    Node() {}
    Node(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int total_cost, int depth_level, int last_move_cost) {
        this->parent = parent;
        this->PositionOfEmptyTile = PositionOfEmptyTile;
        this->total_cost = total_cost;
        this->depth_level = depth_level;
        this->last_move_cost = last_move_cost;
        memcpy(this->puzzle, puzzle, sizeof(this->puzzle));
    }

    Node* newNode(Node* parent, const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int depth_level, int last_move_cost) {
        char newPuzzle[7];
        memcpy(newPuzzle, puzzle, sizeof(newPuzzle));
        swap(newPuzzle[PositionOfEmptyTile], newPuzzle[new_PositionOfEmptyTile]);
        return new Node(parent, newPuzzle, new_PositionOfEmptyTile, 0, depth_level + 1, last_move_cost);
    }

    void printState(const char puzzle[7]) {
        for (int i = 0; i < 7; i++) {
            cout << puzzle[i] << ' ';
        }
        cout << endl;
    }

    void printPath(Node* root) {
        vector<Node*> path;
        while (root != nullptr) {
            path.push_back(root);
            root = root->parent;
        }
        reverse(path.begin(), path.end());
        for (Node* node : path) {
            cout << "Puzzle State: ";
            for (int i = 0; i < 7; i++) cout << node->puzzle[i] << ' ';
            cout << endl;
            if (node->parent != nullptr) {
                cout << "MOVE COST: " << node->last_move_cost << endl;
                cout << "TOTAL COST: " << node->depth_level << endl;
            }
            cout << endl;
        }
    }

    bool GoalCheck(const char puzzle[7]) {
        return solution_goals.find(string(puzzle, puzzle + 7)) != solution_goals.end();
    }

    void DFS(char initialPuzzle[7], int emptyTilePos, int depthLimit) {
        stack<Node*> stack;
        Node* root = new Node(nullptr, initialPuzzle, emptyTilePos, 0, 0, 0);
        stack.push(root);

        unordered_set<string> visited;
        int nodesExpanded = 0;

        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();

            string currentState(current->puzzle, current->puzzle + 7);
            if (visited.find(currentState) != visited.end()) {
                delete current;
                continue;
            }
            visited.insert(currentState);
            nodesExpanded++;

            cout << "Current state at depth " << current->depth_level << ": ";
            printState(current->puzzle);

            if (GoalCheck(current->puzzle)) {
                cout << "Solution found!\nPath to solution:\n";
                printPath(current);
                cout << "Nodes expanded: " << nodesExpanded << endl;

                while (!stack.empty()) {
                    delete stack.top();
                    stack.pop();
                }
                delete current;
                return;
            }

            if (current->depth_level >= depthLimit) {
                delete current;
                continue;
            }

            vector<int> moves = {3, 2, 1, -1, -2, -3};
            for (int move : moves) {
                int newPos = current->PositionOfEmptyTile + move;
                if (newPos >= 0 && newPos < 7) {
                    stack.push(newNode(current, current->puzzle, current->PositionOfEmptyTile, newPos, current->depth_level, abs(move)));
                }
            }
            delete current;
        }
        cout << "No solution found within the depth limit of " << depthLimit << ".\nNodes expanded: " << nodesExpanded << endl;
    }
};

int main() {
    char puzzle[7] = {'B', 'B', 'B', 'W', 'W', 'W', 'K'};
    int emptyTilePosition = 6;
    Node solver;
    solver.DFS(puzzle, emptyTilePosition, 30);
    return 0;
}
