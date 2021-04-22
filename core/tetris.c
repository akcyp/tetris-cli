#include "tetris.h"
void rotate3x3Matrix(int matrix[3][3]) {
  // 1st cycle                                                     |----|
  matrix[0][0] ^= matrix[0][2] ^ matrix[2][2] ^ matrix[2][0]; //   |    v
  matrix[0][2] ^= matrix[2][2] ^ matrix[2][0] ^ matrix[0][0]; //   [][][] --|
  matrix[2][2] ^= matrix[2][0] ^ matrix[0][0] ^ matrix[0][2]; //   [][][]   |
  matrix[2][0] ^= matrix[0][0] ^ matrix[0][2] ^ matrix[2][2]; //   [][][] <-|
  matrix[0][0] ^= matrix[0][2] ^ matrix[2][2] ^ matrix[2][0];
  // 2nd cycle
  matrix[0][1] ^= matrix[1][2] ^ matrix[2][1] ^ matrix[1][0]; //      |----|
  matrix[1][2] ^= matrix[2][1] ^ matrix[1][0] ^ matrix[0][1]; //      |    |
  matrix[2][1] ^= matrix[1][0] ^ matrix[0][1] ^ matrix[1][2]; //   [][][]  |
  matrix[1][0] ^= matrix[0][1] ^ matrix[1][2] ^ matrix[2][1]; //   [][][]<-|
  matrix[0][1] ^= matrix[1][2] ^ matrix[2][1] ^ matrix[1][0]; //   [][][]
};
const int tetrisPieceSchemes[5][3][3] = {
  {
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
  }, {
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
  }, {
    {1, 1, 0},
    {1, 1, 0},
    {0, 0, 0}
  }, {
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 1}
  }, {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
  }
};
const int tetrisPieceSchemesRotateDef[5] = {1, 1, 0, 1, 0};