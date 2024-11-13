#include "Node.h"

#include <queue>

#include <vector>

#include <iostream>

#include <algorithm>

using namespace std;

// Uniform Cost Search class definition
class UCS {
  public:
    //constructor without definitions
    UCS();
  // This is a function in order to solve our puzzle.
  // It takes as definitions the initial state of the puzzle and the position of the empty space.
  void solve(char initial[7], int empty_pos);

  private:
    // vector<string>...practically this works like an array of strings 
    // In order to store all our possible solutions for the puzzle.
    // https://www.geeksforgeeks.org/array-of-strings-in-cpp-5-different-ways-to-create/ 
    vector < string > solution_goals = {
      "WWWBBBK",
      "WWWBBKB",
      "WWWBKBB",
      "WWWKBBB",
      "WWKWBBB",
      "WWKWBBB",
      "WKWWBBB",
      "KWWWBBB",
    };

  // In this function we practically decide to which node we want to move next.
  //choosing the node with the less cost.
  int calculateCost(const char puzzle[7]);

  // In this function we create nodes.
  Node * newNode(Node * parent, const char puzzle[7], int empty_pos, int new_empty_pos, int depth_level, int last_move_cost);

  // This method prints the puzzle e.g. B | B | K | W | W | B | W | at every state that we have chosen to be at.
  void printState(const char puzzle[7]);

  // This function, by printing the variety of costs (Move cost, Total cost) help us to find the path.
  void printPath(Node * root);

  // This function is about to check if we have reached one of our goals in the current state.
  bool Goalcheck(const char puzzle[7]);

  bool compare(Node *lhs, Node *rhs);
};