// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// neldermead_display --
//   Display a Nelder-Mead engine
//
function this = neldermead_display ( this )
  mprintf("Nelder-Mead Object\n");
  this.optbase = optimbase_display ( this.optbase )
  mprintf("Simplex0 : \n");
  str = optimsimplex_tostring ( this.simplex0 );
  for k =1:size(str,1)
    mprintf("%s\n",str(k));
  end
  mprintf("Simplex Optimum : \n");
  str = optimsimplex_tostring ( this.simplexopt );
  for k =1:size(str,1)
    mprintf("%s\n",str(k));
  end
  mprintf("Simplex0 Method : %s\n", string(this.simplex0method));
  mprintf("Simplex0 Length : %s\n", string(this.simplex0length));
  mprintf("Termination Method on simplex size : %s\n", string(this.tolsimplexizemethod));
  mprintf("Termination Absolute Tolerance on simplex size : %s\n", string(this.tolsimplexizeabsolute));
  mprintf("Termination Relative Tolerance on simplex size : %s\n", string(this.tolsimplexizerelative));
  mprintf("Termination Method on standard deviation of function value : %s\n", string(this.tolfstdeviationmethod));
  mprintf("Termination Tolerance on standard deviation of function value : %s\n", string(this.tolfstdeviation));
  mprintf("Termination Method on simplex size + shift of function value : %s\n", string(this.tolssizedeltafvmethod));
  mprintf("Termination Tolerance on shift of function value : %s\n", string(this.toldeltafv));
  mprintf("Method : %s\n", string(this.method));
  mprintf("Reflection Factor (rho) : %s\n", string(this.rho));
  mprintf("Expansion Factor (chi) : %s\n", string(this.chi));
  mprintf("Contraction Factor (gamma) : %s\n", string(this.gamma));
  mprintf("Shrinkage Factor (sigma) : %s\n", string(this.sigma));
  mprintf("Kelley Stagnation : %d\n", this.kelleystagnationflag);
  mprintf("Restart Epsilon : %e\n", this.restarteps);
  mprintf("Restart Step : %s\n", strcat(string(this.restartstep)," "));
  mprintf("Restart Maximum : %d\n", this.restartmax);
endfunction

