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
#include <random> 
using namespace std;

// Node class 
class Node {

  private:
    //Variables declaration
    Node * parent; // A pointer to the parent node...Parent node is the previous node at the tree.
  char puzzle[7]; // Array declaration with 7 positions in order to represent the current state of the puzzle.
  int PositionOfEmptyTile; // It represents the position of the empty square in the puzzle e.g K.
  int Whole_Total_cost; // Whole_Total_cost...from root till current state of the puzzle.
  int last_move_cost; // Cost of the last move made to reach this node.
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
  //Practically a Node has...its parent, the puzzle state, the knowledge of the PositionOfEmptyTile, the total cost from the root till current state, 
  //the depth level of the tree, and the cost of the next move (practically at the next node).
  //constructor with definitions
  Node(Node * parent,const char puzzle[7], int PositionOfEmptyTile, int Whole_Total_cost, int last_move_cost) {
    this -> parent = parent;
    this -> PositionOfEmptyTile = PositionOfEmptyTile;
    this -> Whole_Total_cost = Whole_Total_cost;
    this -> last_move_cost = last_move_cost;
    memcpy(this -> puzzle, puzzle, sizeof(this -> puzzle));
  }

  // In this function we create a new node...practically a child node is created here.
  Node * newNode(Node * parent,const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int Whole_Total_cost, int last_move_cost) { //Inputs at function

    Node * node = new Node(parent, puzzle, new_PositionOfEmptyTile, Whole_Total_cost + last_move_cost, last_move_cost); // Create a new Node (constructor with definitions)

    swap(node -> puzzle[PositionOfEmptyTile], node -> puzzle[new_PositionOfEmptyTile]); // Swap the empty space with the new position
    
    return node; // Returns the new node that is created.
  }

  // This method prints the puzzle e.g. B | B | K | W | W | B | W | at every state that we have chosen to be at.
  void printState(char puzzle[7]) const{
    for (int i = 0; i < 7; i++) {
      cout << puzzle[i] << " | "; // Print the puzzle 
    }
    cout << endl;
  }

  // In this method we practically print the path from the first one node...Till the current one... 
  // By knowing,
  // The MOVE COST from the previous node to this one...
  // And TOTAL COST from the initiall node to this one...
  void printPath(Node * root) const{ //root==current(not root)
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
  bool GoalCheck(char puzzle[7]) { //inputs
    string ConvertCharToString(puzzle, puzzle + 7); //ConvertCharToString...Practically we convert the char InitialPuzzle[7] into a string(Type of Solutions)

    return find(solution_goals.begin(), solution_goals.end(), ConvertCharToString) != solution_goals.end();
  }

 
  void solve(char InitialPuzzle[7], int PositionOfEmptyTile) { //https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/  
    auto compare = [](Node * lhs, Node * rhs) {
        return (lhs->Whole_Total_cost) > (rhs->Whole_Total_cost); // The nodes with the lowest cost are given highest priority and are processed first.
    };
    

    priority_queue<Node*, vector<Node*>, decltype(compare)> pq(compare); //priority queue

    int NodesCount = 0;  

    Node* root = new Node(nullptr, InitialPuzzle, PositionOfEmptyTile, 0, 0);
    NodesCount++; // Increment
    pq.push(root);
        
    vector<pair<int, int>> moves = {
        {-1, 1}, {-2, 2}, {-3, 3}, {1, 1}, {2, 2}, {3, 3}
    };

    // Main Search Loop
    while (!pq.empty()) { // This loop runs as long as there are nodes in the priority queue (pq)
        Node* LowestCostNode = pq.top();
        pq.pop();

        if (GoalCheck(LowestCostNode->puzzle)) { // If the solution is found print it..end
            printPath(LowestCostNode);

            cout << "Total nodes expanded: " << NodesCount << endl; 
            return;
        }

        int PositionOfEmptyTile = LowestCostNode->PositionOfEmptyTile;

        for (const auto& [move, cost] : moves) { // It takes every pair of moves
            int new_PositionOfEmptyTile = PositionOfEmptyTile + move;

            if (new_PositionOfEmptyTile >= 0 && new_PositionOfEmptyTile < 7) {
                Node* child = newNode(LowestCostNode, LowestCostNode->puzzle, PositionOfEmptyTile, new_PositionOfEmptyTile, LowestCostNode->Whole_Total_cost, cost); // Generate new ChildNode
                NodesCount++; // Increment
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
  cout << "!____________________________________________________________________________!" << endl;

  // This is the initial state of our puzzle.
  char InitialPuzzle[7] = {'B','B','B','W','W','W','K'};

  int PositionOfEmptyTile = 6; // InitialPuzzle position of (K)

  Node ucs1; // Here we create an object of UCS class in order to be able to call the functions of UCS.
  ucs1.solve(InitialPuzzle, PositionOfEmptyTile); // Call solve(char InitialPuzzle[7], int PositionOfEmptyTile);...with object ucs1...

  return 0;
}


/*
void printNodeInfo(Node* node) {
    if (node != nullptr) {
        // Print the current puzzle state
        cout << "Puzzle state: ";
        printState(node->puzzle);  // Assuming printState prints the puzzle configuration
        
        // Print the position of the empty tile
        cout << "Position of empty tile: " << node->PositionOfEmptyTile << endl;

        // Print the whole total cost (accumulated cost from the root)
        cout << "Whole total cost from the root: " << node->Whole_Total_cost << endl;
        
        // Print the cost of the last move to reach this node
        cout << "Last move cost: " << node->last_move_cost << endl;
        
        // If the node has a parent, print the parent's total cost
        if (node->parent != nullptr) {
            cout << "Parent node total cost: " << node->parent->Whole_Total_cost << endl;
        } else {
            cout << "This is the root node (no parent)." << endl;
        }

        cout << endl;
    }
}*/

//printNodeInfo(LowestCostNode);

//unordered_set<string> visited;

//visited.insert(string(InitialPuzzle));


                /*string childState(child->puzzle, child->puzzle + 7);
                 Check if the state is already visited
                if (visited.find(childState) == visited.end()) {
                                   // Add to the priority queue
                    visited.insert(childState);   // Mark this state as visited
              }*/