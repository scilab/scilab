// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_outputcmd --
//  This command is called back by the Nelder-Mead
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * simplex : the simplex, as a simplex object
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//  this : the current nmplot object
//
function nmplot_outputcmd ( state , data , this )
  nmplot_log ( this , "nmplot_outputcmd (1)")
  iter = data.iteration
  // Print simplex
  x = optimsimplex_getallx ( data.simplex )
  if this.simplexfn <> "" then
    nbve = optimsimplex_getnbve ( data.simplex )
    n = optimsimplex_getn ( data.simplex )
    mfprintf ( this.simplexhandle , "// Iteration #%d\n", iter )
    mfprintf ( this.simplexhandle , "history($+1) = [\n" )
    for ive = 1:nbve
      mfprintf ( this.simplexhandle , "// Vertex #%d\n", ive )
      for ix = 1:n
        mfprintf ( this.simplexhandle , "%e ", x(ive,ix))
      end
      mfprintf ( this.simplexhandle , "\n")
    end
    mfprintf ( this.simplexhandle , "]\n" )
  end
  // Function value average
  if this.fbarfn <> "" then 
    fbar = optimsimplex_fvmean ( data.simplex )
    mfprintf ( this.fbarhandle , "%d %e\n", iter , fbar )
  end
  // Minimum function value
  if this.foptfn <> "" then
    fopt = data.fval
    mfprintf ( this.fopthandle , "%d %e\n", iter , fopt )
  end
  // Sigma
  if this.sigmafn <> "" then
    sigma = optimsimplex_size ( data.simplex , "sigmaplus" )
    mfprintf ( this.sigmahandle , "%d %e\n", iter , sigma )
  end
endfunction

