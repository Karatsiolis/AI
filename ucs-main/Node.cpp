#include "Node.h"

//constructor without definitions
//Practically everything is set to zero 
Node::Node() {}

//constructor with definitions
Node::Node(Node * parent, const char puzzle[7], int PositionOfEmptyTile, int total_cost, int depth_level, int last_move_cost) {
  this -> parent = parent;
  this -> PositionOfEmptyTile = PositionOfEmptyTile;
  this -> total_cost = total_cost;
  this -> depth_level = depth_level;
  this -> last_move_cost = last_move_cost;
  memcpy(this -> puzzle, puzzle, sizeof(this -> puzzle));
} 