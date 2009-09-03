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
  n = this.n
  if (~isdef('method','local')) then
    method = "sigmaplus";
  end
  select method
  case "Nash" then
    // TODO : fix the formula and use norm - 1 instead of euclidian norm (check that Nash indeed use norm 1)
    ssize = 0.0;
    for iv = 2:this.nbve
      ssize = ssize + norm(this.x(:,iv) - this.x(:,1));
    end
  case "diameter" then
    ssize = 0.0;
    for i = 1:this.nbve
      for j = 1:this.nbve
        ssize = max(ssize , norm(this.x(:,i) - this.x(:,j)));
      end
    end
  case "sigmaplus" then
    ssize = 0.0;
    for j = 2:this.nbve
      ssize = max(ssize , norm(this.x(:,j) - this.x(:,1)));
    end
  case "sigmaminus" then
    ssize = 1.e307;
    for j = 2:this.nbve
      ssize = min(ssize , norm(this.x(:,j) - this.x(:,1)));
    end
  else
    errmsg = sprintf("Unknown simplex size method %s",method)
    error(errmsg)
  end
endfunction

