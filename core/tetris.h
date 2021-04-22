struct tetrisPiece {
  int block[3][3];
  int canRotate;
  struct {
    int x;
    int y;
  } position;
};
void rotate3x3Matrix(int matrix[3][3]);
const int tetrisPieceSchemes[5][3][3];
const int tetrisPieceSchemesRotateDef[5];