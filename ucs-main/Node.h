#include <cstring>

#include <climits>

#include <vector>

#include <string>

// Node class 
class Node {
  public:

  //constructors
  //constructor without definitions
  Node();
  //constructor with definitions
  Node(Node * parent, const char puzzle[7], int empty_pos, int total_cost, int depth_level, int move_cost);

  //Variables declaration
  Node * parent; // A pointer to the parent node...Parent node is the previous node at the tree
  char puzzle[7]; // Array declaration with 7 positions in order to represent the current state of the puzzle.
  int empty_pos; // It represents the position of the empty square in the puzzle e.g K.
  int total_cost; // Total cost to reach this node (could represent the cost of moves)
  int depth_level; // Depth level of the node in the search tree (how many moves from the root)
  int last_move_cost; // Cost of the last move made to reach this node
};