// Main concept for the implementation of UCS belongs to:
// Compile: g++ main.cpp UCS.cpp Node.cpp -o output
// Execute: ./output
// Project Authors:...
#include <unordered_set>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cstring>
using namespace std;

// Node class 
class Node {

  private:
    //Variables declaration
    Node * parent; // A pointer to the parent node...Parent node is the previous node at the tree
  char puzzle[7]; // Array declaration with 7 positions in order to represent the current state of the puzzle.
  int PositionOfEmptyTile; // It represents the position of the empty square in the puzzle e.g K.
  int cost; // Total cost to reach this node (could represent the cost of moves)
  int Whole_Total_cost; // Depth level of the node in the search tree (how many moves from the root)
  int last_move_cost; // Cost of the last move made to reach this node
  vector < string > solution_goals = {
    "WWWBBBK",
    "WWWBBKB",
    "WWWBKBB",
    "WWWKBBB",
    "WWKWBBB",
    "WKWWBBB",
    "KWWWBBB",

  };

  public:
    //constructors
    //constructor without definitions
    //Practically everything is set to zero 
    Node() {}
  //constructor with definitions
  //Practically a Node has...its parent, the puzzle state, the knowledge of the PositionOfEmptyTile, the total cost from the root till current state, 
  //the depth level of the tree, and the cost of the next move (practically at the next node).
  //constructor with definitions
  Node(Node * parent,const char puzzle[7], int PositionOfEmptyTile, int cost, int Whole_Total_cost, int last_move_cost) {
    this -> parent = parent;
    this -> PositionOfEmptyTile = PositionOfEmptyTile;
    this -> cost = cost;
    this -> Whole_Total_cost = Whole_Total_cost;
    this -> last_move_cost = last_move_cost;
    memcpy(this -> puzzle, puzzle, sizeof(this -> puzzle));
  }

  // In this function we create a new node...practically a child node is created here.
  Node * newNode(Node * parent,const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int Whole_Total_cost, int last_move_cost) {
    Node * node = new Node(parent, puzzle, new_PositionOfEmptyTile, INT_MAX, Whole_Total_cost + last_move_cost, last_move_cost); // Create a new Node
    swap(node -> puzzle[PositionOfEmptyTile], node -> puzzle[new_PositionOfEmptyTile]); // Swap the empty space with the new position
    return node; // Returns the new node that is created.
  }

  // In this function we practically decide to which node we want to move next.
  //choosing the node with the less cost.
  int calculateCost(const char puzzle[7]) {
    int minimum_cost_expansion = INT_MAX; //https://www.geeksforgeeks.org/int_max-int_min-cc-applications // minimum_cost_expansion will be assigned to the lowest cost.
    for (string goal: solution_goals) { // Iterate through all goal states
      int cost = 0; // Initialize total cost for the current goal state
      for (int i = 0; i < 7; i++) { // Compare the current puzzle state with the goal state
        if (puzzle[i] != goal[i]) {
          cost++; // If the current puzzle state is different from the goal state, increment the total cost.
        }
      }
      minimum_cost_expansion = min(minimum_cost_expansion, cost); // Update minimum_cost_expansion if the current cost is lower
    }
    return minimum_cost_expansion; // Return the minimum_cost_expansion found.
  }

  // This method prints the puzzle e.g. B | B | K | W | W | B | W | at every state that we have chosen to be at.
  void printState(const char puzzle[7]) {
    for (int i = 0; i < 7; i++) {
      cout << puzzle[i] << " | "; // Print the puzzle 
    }
    cout << endl;
  }

  // In this method we practically print the path from the first one node...Till the current one... 
  // By knowing,
  // The MOVE COST from the previous node to this one...
  // And TOTAL COST from the initiall node to this one...
  void printPath(Node * root) {
    if (root == nullptr) return; // Base case: if the root is null, return
    printPath(root -> parent); // Recursively print the path to the parent node
    printState(root -> puzzle); // Print the current state of the puzzle.
    // Move cost and total cost here is zero.
    if (root -> parent != nullptr) { // If the current node has a parent
      cout << "The MOVE COST from the previous node to this one is : " << root -> last_move_cost << endl; // Print the cost of the last move to the current one
      cout << "The TOTAL COST from the initiall node to this one is: " << root -> Whole_Total_cost << endl; // Print the total cost of all the destination.
      //from the root till the current state.
    }
    cout << endl;
  }

  // This function is about to check if we have reached one of our goals in the current state.
  bool GoalCheck(const char puzzle[7]) {

    string curr(puzzle, puzzle + 7);

    return find(solution_goals.begin(), solution_goals.end(), curr) != solution_goals.end();
  }

  //https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/ 
    void solve(char InitialPuzzle[7], int PositionOfEmptyTile) {
        auto compare = [](Node * lhs, Node * rhs) {
            return (lhs->cost + lhs->Whole_Total_cost) > (rhs->cost + rhs->Whole_Total_cost);
        };

        priority_queue<Node*, vector<Node*>, decltype(compare)> pq(compare);
        //unordered_set<string> visited;
        int NodesCount = 0;

        Node* root = new Node(nullptr, InitialPuzzle, PositionOfEmptyTile, calculateCost(InitialPuzzle), 0, 0);
        pq.push(root);
       // visited.insert(string(InitialPuzzle));

        vector<pair<int, int>> moves = {
            {-1, 1}, {-2, 2}, {-3, 3}, {1, 1}, {2, 2}, {3, 3}
        };

        srand(time(0));

        while (!pq.empty()) {
            // COllect lowest cost nodes, put them in vector.
            vector<Node*> sameCostNodes;
            int lowestCost = pq.top()->cost + pq.top()->Whole_Total_cost;

            while (!pq.empty() && pq.top()->cost + pq.top()->Whole_Total_cost == lowestCost) {
                sameCostNodes.push_back(pq.top());
                pq.pop();
            }

            // Randomly select one of the lowest cost nodes.
            Node* min = sameCostNodes[rand() % sameCostNodes.size()];

            if (GoalCheck(min->puzzle)) {
                printPath(min);
                cout << "Total nodes expanded: " << NodesCount << endl;
                return;
            }

            int PositionOfEmptyTile = min->PositionOfEmptyTile;

            for (const auto& [move, cost] : moves) {
                int new_PositionOfEmptyTile = PositionOfEmptyTile + move;

                if (new_PositionOfEmptyTile >= 0 && new_PositionOfEmptyTile < 7) {
                    Node* child = newNode(min, min->puzzle, PositionOfEmptyTile, new_PositionOfEmptyTile, min->Whole_Total_cost, cost);
                    NodesCount++;
                    child->cost = calculateCost(child->puzzle);
                    pq.push(child);

                    //string childState(child->puzzle, child->puzzle + 7);
                   // if (visited.find(childState) == visited.end()) {
                     //   pq.push(child);
                    //    visited.insert(childState);
                   // }
                }
            }
        }

        cout << "No solution can be found." << endl;
        cout << "Total nodes expanded: " << NodesCount << endl;
    }

};

int main() {
  cout << "Welcome to the first AI project that implements Uniform Cost Search Algorithm!" << endl;
  // This is the initial state of our puzzle.
  char InitialPuzzle[7] = {'B','B','B','W','W','W','K'};


  int PositionOfEmptyTile = 6; // InitialPuzzle position of (K)

  Node ucs1; // Here we create an object of UCS class in order to be able to call the functions of UCS.
  ucs1.solve(InitialPuzzle, PositionOfEmptyTile); // Call solve(char InitialPuzzle[7], int PositionOfEmptyTile);...with object c1...

  return 0;
}