// Compile: g++ ucs.cpp -o output
// Execute: ./output
// https://www.geeksforgeeks.org/int_max-int_min-cc-applications 
// https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/ 
#include <unordered_set>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

using namespace std;

// Node class 
class Node {
  private: 
  Node * parent; // A pointer to the parent node...Parent node is the previous node at the tree
  char puzzle[7]; // Array declaration with 7 positions in order to represent the current state of the puzzle.
  int PositionOfEmptyTile; // It represents the position of the empty square in the puzzle e.g K.
  int Whole_Total_cost; // Whole_Total_cost for Basic comparison
  int last_move_cost; // Cost of the last move made to reach this node
  vector < string > solution_goals = { //An array of strings with all the possible the solutions.
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
  //Practically a Node has...its parent, the puzzle state, the knowledge of the PositionOfEmptyTile,
  //the Whole_Total_cost of the tree, and the cost of the next move (practically to the next node).
  //constructor with definitions
  Node(Node * parent, const char puzzle[7], int PositionOfEmptyTile, int Whole_Total_cost, int last_move_cost) {
    this -> parent = parent;
    this -> PositionOfEmptyTile = PositionOfEmptyTile;
    this -> Whole_Total_cost = Whole_Total_cost;
    this -> last_move_cost = last_move_cost;
    memcpy(this -> puzzle, puzzle, sizeof(this -> puzzle));
  }

  // In this function we create a new node...practically a child node is created here.
  Node * newNode(Node * parent, const char puzzle[5], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int last_move_cost) { //Inputs at function
    int new_Whole_Total_cost; // In order to update new_Whole_Total_cost
    new_Whole_Total_cost = parent -> Whole_Total_cost + last_move_cost;
    Node * node = new Node(parent, puzzle, new_PositionOfEmptyTile, new_Whole_Total_cost, last_move_cost); // Create a new Node (constructor with definitions)
    swap(node -> puzzle[PositionOfEmptyTile], node -> puzzle[new_PositionOfEmptyTile]); // Swap the empty space with the new position
    return node; // Returns the new node that is created.
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
    printPath(root -> parent);  // Recursively print the path to the parent node
    printState(root -> puzzle); // Print the current state of the puzzle.
    if (root -> parent != nullptr) { // If the current node has a parent
      cout << "move cost: " << root -> last_move_cost; // Print the cost of the last move to the current one
      cout << ", total cost : " << root -> Whole_Total_cost << endl; // Print the total cost of all the destination.
    }
    cout << endl;
  }

  // This function is about to check if we have reached one of our goals in the current state.
  bool GoalCheck(const char puzzle[7]) {
    string ConvertCharToString(puzzle, puzzle + 7); // ConvertCharToString...Practically we convert the char InitialPuzzle[7] into a string(Type of Solutions)
    return find(solution_goals.begin(), solution_goals.end(), ConvertCharToString) != solution_goals.end();
  }

  void solve(char InitialPuzzle[7], int PositionOfEmptyTile) {
    auto compare = [](Node * lhs, Node * rhs) { //Comparison for the less Whole_Total_cost
      return lhs -> Whole_Total_cost > rhs -> Whole_Total_cost;
    };

    priority_queue < Node * , vector < Node * > , decltype(compare) > pq(compare); //Priority queue initialization
    int NodesCount = 1;

    Node * root = new Node(nullptr, InitialPuzzle, PositionOfEmptyTile, 0, 0); //Create the root
    pq.push(root); // Push root in queue

    vector < pair < int, int >> moves = { // Moves and costs
      {-1, 1}, {-2, 2}, {-3, 3}, {1, 1}, {2, 2}, {3, 3}
    };

    srand(time(0)); //Random

    while (!pq.empty()) {
      vector < Node * > sameCostNodes;
      int lowestCost = pq.top() -> Whole_Total_cost;

        while (!pq.empty() && pq.top() -> Whole_Total_cost == lowestCost) { // Node with same cost
        sameCostNodes.push_back(pq.top());
        pq.pop();
      }

      Node * min = sameCostNodes[rand() % sameCostNodes.size()]; // Random choose

      if (GoalCheck(min -> puzzle)) { // If goal reached...Finish
        printPath(min);
        cout << "Total nodes expanded: " << NodesCount << endl;
        return;
      }

      int PositionOfEmptyTile = min -> PositionOfEmptyTile; // Update PositionOfEmptyTile

      for (const auto & [move, move_cost]: moves) { // At all the possible moves
        int new_PositionOfEmptyTile = PositionOfEmptyTile + move; // Update new_PositionOfEmptyTile

        if (new_PositionOfEmptyTile >= 0 && new_PositionOfEmptyTile < 7) {
          Node * child = newNode(min, min -> puzzle, PositionOfEmptyTile, new_PositionOfEmptyTile, move_cost); // Create new child
          NodesCount++; // All the nodes
          pq.push(child); // Push child into queue
        }
      }
    }

    cout << "No solution can be found." << endl;
    cout << "Total nodes expanded: " << NodesCount << endl;
  }
};

int main() {
  cout << "Welcome to the first AI project that implements Uniform Cost Search Algorithm!" << endl;
  char InitialPuzzle[7] = {'B','B','B','W','W','W','K'};

  int PositionOfEmptyTile = 6; // InitialPuzzle position of (K)

  Node c1; // Here we create an object of UCS class in order to be able to call the functions of UCS.
  c1.solve(InitialPuzzle, PositionOfEmptyTile); // Call solve(char InitialPuzzle[7], int PositionOfEmptyTile);...with object c1...

  return 0;
}