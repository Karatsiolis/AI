// Main concept for the implementation of UCS belongs to:
// Compile: g++ main.cpp UCS.cpp Node.cpp -o output
// Execute: ./output
// Project Authors:...
#include "UCS.h"
using namespace std;

int main() {
  cout<<"Welcome to the first AI project that implements Uniform Cost Search Algorithm!"<<endl;
  // This is the initial state of our puzzle.
  char InitialPuzzle[7] = {'B','B','B','W','W','W','K'};
  
  int PositionOfEmptyTile = 6; // InitialPuzzle position of (K)

  UCS ucs1; // Here we create an object of UCS class in order to be able to call the functions of UCS.
  ucs1.solve(InitialPuzzle, PositionOfEmptyTile); // Call solve(char InitialPuzzle[7], int PositionOfEmptyTile);...with object c1...

  return 0;
}