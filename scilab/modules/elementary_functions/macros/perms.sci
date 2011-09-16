// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = perms(x)

  // PERMS function
  // Given a vector x (size: 1*n or n*1), this function returns all the permutations of x (i.e n! permutations)
  // Inputs : 
  //  x : scalars/vectors
  // output :
  // y : matrix
  // F.Belahcene

  [lhs, rhs] = argn(0);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"perms", 1));
  end

  if size(x,"*")==1 | size(x,"*")==0 then
    y = x;
  elseif size(x,1)<>1 & size(x,2)<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"),"perms"));
  else
    xsize = size(x, "*");
    indrec = perms(1:xsize-1);
    ind = [ones(prod(1:xsize-1),1)*xsize, indrec];
    for j = xsize-1:-1:1
      indrec(indrec==j) = j + 1;
      ind = [ind; [ones(prod(1:xsize-1),1)*j,indrec]];
    end
    y = matrix(x(ind), size(ind));
  end

endfunction

