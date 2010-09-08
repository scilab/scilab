//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file is distributed under the same license as the Scilab package.
//

function result = rootsdemo(p)
//   A root-finder algorithm based on the eigenvalues of the 
//   companion matrix of the polynomial p
  A=companion(p);
  result = spec(A)
endfunction

