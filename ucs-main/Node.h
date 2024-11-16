#include <cstring>
#include <climits>
#include <vector>
#include <string>

// Node class 
class Node {
  public:

  //Variables declaration
  Node * parent; // A pointer to the parent node...Parent node is the previous node at the tree
  char puzzle[7]; // Array declaration with 7 positions in order to represent the current state of the puzzle.
  int PositionOfEmptyTile; // It represents the position of the empty square in the puzzle e.g K.
  int total_cost; // Total cost to reach this node (could represent the cost of moves)
  int depth_level; // Depth level of the node in the search tree (how many moves from the root)
  int last_move_cost; // Cost of the last move made to reach this node

  //constructors
  //constructor without definitions
  Node();
  //constructor with definitions
  //Practically a Node has...its parent, the puzzle state, the knowledge of the PositionOfEmptyTile, the total cost from the root till current state, the depth level of the tree, and the cost of the next move (practically at the next node).
  Node(Node * parent, const char puzzle[7], int PositionOfEmptyTile, int total_cost, int depth_level, int move_cost);
};