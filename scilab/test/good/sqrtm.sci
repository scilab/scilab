function x=sqrtm(a)
//   sqrtm - computes the matrix square root. 
//%CALLING SEQUENCE
//   x=sqrtm(a)
//%PARAMETERS
//   a   : square hermitian or diagonalizable matrix
//   x   : square  matrix
//!
// Copyright INRIA
x=%s_pow(a,0.5);
endfunction
