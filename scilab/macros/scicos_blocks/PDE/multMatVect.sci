function y = multMatVect(M, x)
// Copyright INRIA
// développé par EADS-CCR
// Function that multiply a sparse nxp matrix of double by
// a nx1 vector of string, resulting in a px1 vector of string
// Input:
// - M a nxp sparse matrix of double
// - x a nx1 vector of string
// Ouput:
// - y a px1 vector of string

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
  mn_x = size(x);
  if(mn_x(1) <> mn_M(2)) then
    error('Incompatible dimensions');
  end
  // Perform the multiplication
  y = emptystr(mn_M(1), 1);
  for n=1:length(v)
    y(ij(n, 1)) = addfv(y(ij(n, 1)) , mulfv(msprintfv(v(n)), x(ij(n, 2))));
  end
  if (y == '') then
    y=[];
  end
endfunction
