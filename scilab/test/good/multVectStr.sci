function y = multVectStr(M, x)
// Copyright INRIA
// développé par EADS-CCR
// Function that multiply a sparse 1xn of double by
// a 1x1 string, resulting in a nx1 vector of string
// Input:
// - M a 1xn sparse vector of double
// - x a string
// Ouput:
// - y a nx1 vector of string

// Check the syntax
  if(argn(1) <> 1) then
    error('Usage: y = multMatVect(M, x)');
  end
  if(M == []) then
    y=[];
    return;
  end
  // Check the dimensions
  [ij,v,mn_M]=spget(M);
  // Perform the multiplication
  y = emptystr(1, mn_M(2));
  for n=1:length(v)
    y(ij(n, 2)) = mulfv(msprintfv(v(n)) ,x);
  end
  if (y == '') then
    y=[];
  end
endfunction

