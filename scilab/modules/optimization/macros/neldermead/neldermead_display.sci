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
function neldermead_display ( this )
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
  mprintf("Simplex0, Pfeffer, Delta Usual : %s\n", string(this.simplex0deltausual));
  mprintf("Simplex0, Pfeffer, Delta Zero : %s\n", string(this.simplex0deltazero));
  mprintf("Simplex0, Given, Coords :\n");
  disp(this.coords0)
  // Termination
  mprintf("Termination on simplex size : %s\n", string(this.tolsimplexizemethod));
  mprintf("Termination on simplex size, Absolute Tolerance : %s\n", string(this.tolsimplexizeabsolute));
  mprintf("Termination on simplex size, Relative Tolerance : %s\n", string(this.tolsimplexizerelative));
  mprintf("Termination on simplex size, Initial Simplex Size : %s\n", string(this.simplexsize0));
  mprintf("Termination on simplex size + Delta of function value : %s\n", string(this.tolssizedeltafvmethod));
  mprintf("Termination on simplex size + Delta of function value, Absolute Tolerance on Delta F : %s\n", string(this.toldeltafv));
  mprintf("Termination on Kelley''s Stagnation : %s\n", string(this.kelleystagnationflag));
  mprintf("Termination on Kelley''s Stagnation, Normalization : %s\n", string(this.kelleynormalizationflag));
  mprintf("Termination on Kelley''s Stagnation, Alpha0 : %s\n", string(this.kelleystagnationalpha0));
  mprintf("Termination on Kelley''s Stagnation, Alpha : %s\n", string(this.kelleyalpha));
  mprintf("Termination by Box : %s\n", string(this.boxtermination));
  mprintf("Termination by Box, Absolute Tolerance on Function: %s\n", string(this.boxtolf));
  mprintf("Termination by Box, Maximum Number of Consecutive Match : %s\n", string(this.boxnbmatch));
  mprintf("Termination by Box, Current Number of Consecutive Match : %s\n", string(this.boxkount));
  mprintf("Termination on Variance : %s\n", string(this.tolvarianceflag));
  mprintf("Termination on Variance, Absolute Tolerance : %s\n", string(this.tolabsolutevariance));
  mprintf("Termination on Variance, Relative Tolerance : %s\n", string(this.tolrelativevariance));
  mprintf("Termination on Variance, Variance of Initial Simplex : %s\n", string(this.variancesimplex0));
  // Algorithms parameters
  mprintf("Method : %s\n", string(this.method));
  mprintf("Reflection Factor (rho) : %s\n", string(this.rho));
  mprintf("Expansion Factor (chi) : %s\n", string(this.chi));
  mprintf("Contraction Factor (gamma) : %s\n", string(this.gamma));
  mprintf("Shrinkage Factor (sigma) : %s\n", string(this.sigma));
  mprintf("Kelley Stagnation : %s\n", string(this.kelleystagnationflag));
  mprintf("Restart Epsilon : %e\n", this.restarteps);
  mprintf("Restart Step : %s\n", _strvec(this.restartstep));
  mprintf("Restart Maximum : %d\n", this.restartmax);
  mprintf("Restart Simplex Method : %s\n", string(this.restartsimplexmethod));
  mprintf("Restart Flag : %s\n", string(this.restartflag));
  mprintf("Restart Number : %s\n", string(this.restartnb));
  mprintf("Restart Detection Method : %s\n", string(this.restartdetection));
  mprintf("Startup Flag : %s\n", string(this.startupflag));
  mprintf("Automatic Checking of Cost Function : %s\n", string(this.checkcostfunction));
  mprintf("Box, Number of Points  : %s\n", string(this.boxnbpoints));
  mprintf("Box, Current Number of Points  : %s\n", string(this.boxnbpointseff));
  mprintf("Box, Scaling, Factor  : %s\n", string(this.boxineqscaling));
  mprintf("Box, Scaling, Method  : %s\n", string(this.scalingsimplex0));
  mprintf("Box, Scaling, Minimum : %s\n", string(this.guinalphamin));
  mprintf("Box, Bounds Parameter: %s\n", string(this.boxboundsalpha));
  mprintf("Box, Reflection Coefficient : %s\n", string(this.boxreflect));


endfunction

  //
  // _strvec --
  //  Returns a string for the given vector.
  //
  function str = _strvec ( x )
      str = strcat(string(x)," ")
  endfunction

