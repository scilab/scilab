// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimbase_display --
//   Display the current settings in the console.
//
function this = optimbase_display ( this )
  mprintf("Optimization Object\n");
  mprintf("Verbose logging : %s\n", string(this.verbose));
  mprintf("Verbose Termination : %s\n", string(this.verbosetermination));
  mprintf("Store History : %s\n", string(this.storehistory));
  mprintf("Number of variables : %s\n", string(this.numberofvariables));
  mprintf("Initial Guess :\n");
  x0 = optimbase_cget (this,"-x0")
  disp(x0)
  mprintf("Initial Function Value :%e\n",this.fx0);
  mprintf("Optimum Parameters :\n");
  disp(this.xopt)
  mprintf("Optimum Function Value :%e\n",this.fopt);
  mprintf("Number of iterations : %d\n", this.iterations);
  mprintf("Maximum number of iterations : %s\n", string(this.maxiter));
  mprintf("Number function evaluations : %d\n", this.funevals);
  mprintf("Maximum number of function evaluations : %s\n", string(this.maxfunevals));
  mprintf("Termination Method on function value : %s\n", string(this.tolfunmethod));
  mprintf("Termination Absolute Tolerance on function value : %s\n", string(this.tolfunabsolute));
  mprintf("Termination Relative Tolerance on function value : %s\n", string(this.tolfunrelative));
  mprintf("Termination Method on x : %s\n", string(this.tolxmethod));
  mprintf("Termination Absolute Tolerance on x : %s\n", string(this.tolxabsolute));
  mprintf("Termination Relative Tolerance on x : %s\n", string(this.tolxrelative));
  mprintf("Optimization Status : %s\n", this.status);
endfunction

