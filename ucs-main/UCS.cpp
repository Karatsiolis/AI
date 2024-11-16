#include "UCS.h"
#include <unordered_set>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
using namespace std;

// Initiallizing the constructor UCS.h class..Practically everything is set to zero.
UCS::UCS() {}

// In this function we practically decide to which node we want to move next.
//choosing the node with the less cost.
int UCS::calculateCost(const char puzzle[7]) {
  int minimum_cost_expansion = INT_MAX; //https://www.geeksforgeeks.org/int_max-int_min-cc-applications // minimum_cost_expansion will be assigned to the lowest cost.
  for (string goal: solution_goals) { // Iterate through all goal states
    int total_cost = 0; // Initialize total cost for the current goal state
    for (int i = 0; i < 7; i++) { // Compare the current puzzle state with the goal state
      if (puzzle[i] != goal[i]) {
        total_cost++; // If the current puzzle state is different from the goal state, increment the total cost.
      }
    }
    minimum_cost_expansion = min(minimum_cost_expansion, total_cost); // Update minimum_cost_expansion if the current total_cost is lower
  }
  return minimum_cost_expansion; // Return the minimum_cost_expansion found.
}

// In this function we create a new node...practically a child node is created here.
Node * UCS::newNode(Node * parent,const char puzzle[7], int PositionOfEmptyTile, int new_PositionOfEmptyTile, int depth_level, int last_move_cost) {
  Node * node = new Node(parent, puzzle, new_PositionOfEmptyTile, INT_MAX, depth_level + last_move_cost, last_move_cost); // Create a new Node
  swap(node -> puzzle[PositionOfEmptyTile], node -> puzzle[new_PositionOfEmptyTile]); // Swap the empty space with the new position
  return node; // Returns the new node that is created.
}

// This method prints the puzzle e.g. B | B | K | W | W | B | W | at every state that we have chosen to be at.
void UCS::printState(const char puzzle[7]) {
  for (int i = 0; i < 7; i++) {
    cout << puzzle[i] << " | "; // Print the puzzle 
  }
  cout << endl;
}

// In this method we practically print the path from the first one node...Till the current one... 
// By knowing,
// The MOVE COST from the previous node to this one...
// And TOTAL COST from the initiall node to this one...

void UCS::printPath(Node * root) {
  if (root == nullptr) return; // Base case: if the root is null, return
  printPath(root -> parent); // Recursively print the path to the parent node
  printState(root -> puzzle); // Print the current state of the puzzle.
  // Move cost and total cost here is zero.
  if (root -> parent != nullptr) { // If the current node has a parent
    cout << "The MOVE COST from the previous node to this one is : " << root -> last_move_cost << endl; // Print the cost of the last move to the current one
    cout << "The TOTAL COST from the initiall node to this one is: " << root -> depth_level << endl; // Print the total cost of all the destination.
    //from the root till the current state.
  }
  cout << endl;
}

// This function is about to check if we have reached one of our goals in the current state.
bool UCS::GoalCheck(const char puzzle[7]) {

  string curr(puzzle, puzzle + 7);

  return find(solution_goals.begin(), solution_goals.end(), curr) != solution_goals.end();
}

//https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/ 
void UCS::solve(char InitialPuzzle[7], int PositionOfEmptyTile) {

  auto compare = [](Node * lhs, Node * rhs) {
    return (lhs -> total_cost + lhs -> depth_level) > (rhs -> total_cost + rhs -> depth_level);
  };

  priority_queue < Node * , vector < Node * > , decltype(compare) > pq(compare); // https://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator 

  unordered_set < string > visited;

  Node * root = new Node(nullptr, InitialPuzzle, PositionOfEmptyTile, calculateCost(InitialPuzzle), 0, 0);

  pq.push(root);

  visited.insert(string(InitialPuzzle));

  vector < pair < int, int >> moves = {
    {-1, 1},{-2,2},{-3,3},{1,1},{2,2},{3,3}  //https://stackoverflow.com/questions/13406790/filling-a-vector-of-pairs 
  };

  int NodesCount = 0; 

  while (!pq.empty()) {

    Node * min = pq.top();
    pq.pop();

    NodesCount++;  

    if (GoalCheck(min -> puzzle)) {
      printPath(min);
      cout << "Total nodes expanded: " << NodesCount << endl;
      return;
    }

    int PositionOfEmptyTile = min -> PositionOfEmptyTile;

    for (const auto & [move, total_cost]: moves) {

      int new_PositionOfEmptyTile = PositionOfEmptyTile + move;

      if (new_PositionOfEmptyTile >= 0 && new_PositionOfEmptyTile < 7) {

        Node * child = newNode(min, min -> puzzle, PositionOfEmptyTile, new_PositionOfEmptyTile, min -> depth_level, total_cost);

        child -> total_cost = calculateCost(child -> puzzle);

        pq.push(child);
      }
    }
  }

  cout << "No solution can be found." << endl;
}
