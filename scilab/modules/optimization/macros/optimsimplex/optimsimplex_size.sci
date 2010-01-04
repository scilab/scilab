// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_size --
//   Returns the size of the simplex.
// Arguments, input
//   method : the method to use to compute the simplex, "Nash"
//     "diameter", "sigmaplus" or "sigmaminus" (default is "sigmaplus")
// References
//   [1] Compact Numerical Methods For Computers
//   Linear Algebra and Function Minimization
//   J.C. Nash
//   1990
//   Chapter 14. Direct Search Methods
//   [2] Iterative Methods for Optimization
//   C.T. Kelley
//   1999
//   Chapter 6., section 6.2
//
function ssize = optimsimplex_size ( this , method )
  n = this.n;
  nv = this.nbve;
  if (~isdef('method','local')) then
    method = "sigmaplus";
  end
  select method
  case "Nash" then
    v1 = this.x(1,:) .*. ones(nv-1,1);
    edges = this.x(2:nv,:) - v1;
    abedges = abs(edges);
    n1 = sum(abedges,"c");
    ssize = sum ( n1 );
  case "diameter" then
    ssize = 0.0;
    for i = 1:nv
      vi = this.x(i,:) .*. ones(nv,1);
      edges = vi - this.x(1:nv,:);
      n2 = sqrt ( sum ( edges.^2 , "c" ) );
      ssize = max ( max( n2 ) , ssize );
    end
  case "sigmaplus" then
    v1 = this.x(1,:) .*. ones(nv-1,1);
    edges = this.x(2:nv,:) - v1;
    n2 = sqrt ( sum ( edges.^2 , "c" ) );
    ssize = max ( n2 );
  case "sigmaminus" then
    v1 = this.x(1,:) .*. ones(nv-1,1);
    edges = this.x(2:nv,:) - v1;
    n2 = sqrt ( sum ( edges.^2 , "c" ) );
    ssize = min ( n2 );
  else
    errmsg = msprintf(gettext ( "%s: Unknown simplex size method %s") , "optimsimplex_size",method)
    error(errmsg)
  end
endfunction

