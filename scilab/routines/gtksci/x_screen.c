#include "../machine.h" 

#define SCITRW ";lines(%3d,%3d);\n"

/* JPC */


void Size2Scilab(rows, cols)
  int rows, cols;
{
  C2F(scilines)(&rows,&cols);
}

