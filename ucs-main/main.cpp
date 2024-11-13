// Main concept for the implementation of UCS belongs to:
// Compile: g++ main.cpp UCS.cpp Node.cpp -o output
// Execute: ./output

// Project Authors:...

#include "UCS.h"

int main() {
  // This is the initiall state of our puzzle.
  char initial[7] = {
    'B',
    'B',
    'B',
    'W',
    'W',
    'W',
    'K'
  };
  int empty_pos = 6; // Initial position of (K)

  UCS c1; // Here we create an object of UCS class in order to be able to call the functions of UCS.
  c1.solve(initial, empty_pos); // Call solve(char initial[7], int empty_pos);...with c1...

  return 0;
}