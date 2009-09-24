// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_new --
//   Creates a new Nelder-Mead object.
//
function newobj = neldermead_new ()
  newobj = tlist(["T_NELDERMEAD",...
    "optbase","method",...
    "simplex0","simplex0method","simplex0length",...
    "rho","chi","gamma","sigma",...
    "tolfstdeviation","tolfstdeviationmethod",...
    "tolsimplexizeabsolute","tolsimplexizerelative","tolsimplexizemethod", "simplexsize0", ...
    "toldeltafv","tolssizedeltafvmethod",...
    "historysimplex", ...
    "coords0",...
    "simplex0deltausual","simplex0deltazero", ...
    "restartsimplexmethod",...
    "simplexopt","restartmax" , "restarteps", ...
    "restartstep","kelleystagnationflag",...
    "kelleynormalizationflag","kelleystagnationalpha0", ...
    "kelleyalpha","restartnb","restartflag","restartdetection" , ...
    "startupflag" , ...
    "boxnbpoints" , "boxnbpointseff" , "nbineqloops" , "ineqscaling" , ...
    "vectorizefunction" ]);
  newobj.optbase = optimbase_new();
  // Possible values "variable", "fixed".
  newobj.method = "variable";
  newobj.simplex0 = optimsimplex_new ( );
  // Possible values : "axes", "spendley", "pfeffer" 
  newobj.simplex0method = "axes";
  newobj.simplex0length = 1.0;
  // Reflection factor : rho
  newobj.rho = 1.0;
  // Expansion factor : chi
  newobj.chi = 2.0;
  // Contraction factor : gamma
  newobj.gamma = .5;
  // Shrinkage factor : sigma
  newobj.sigma = .5;
  // The tolerance for the standard deviation
  newobj.tolfstdeviation = 0.0;
  // Possible values : "disabled", "enabled"
  newobj.tolfstdeviationmethod = "disabled";
  // The absolute tolerance for the simplex size
  newobj.tolsimplexizeabsolute = 0.0;
  // The relative tolerance for the simplex size
  newobj.tolsimplexizerelative = %eps;
  // Possible values : "disabled", "enabled"
  // TODO : turn this on by default. 
  // Note :
  //   If the simplex method converges, the simplex size is near zero.
  newobj.tolsimplexizemethod = "enabled";
  // The tolerance for the function value delta
  newobj.toldeltafv = %eps;
  // Possible values : "disabled", "enabled"
  newobj.tolssizedeltafvmethod = "disabled";
  // The value used in Pfeffer method initial simplex computation for non-zero parameters
  newobj.simplex0deltausual = 0.05;
  // The value used in Pfeffer method initial simplex computation for zero parameters
  newobj.simplex0deltazero = 0.0075;
  // The coordinates of the initial simplex, given by the user
  newobj.coords0 = [];
  // The Kelley stagnation detection in termination criteria :  0/1
  // (i.e. sufficient decrease of function value)
  newobj.kelleystagnationflag = 0
  // The Kelley stagnation detection parameter
  newobj.kelleystagnationalpha0 = 1.e-4
  // The Kelley stagnation detection can be normalized or not.
  // Note: 
  // * in the 1997 paper "Detection and Remediation of Stagnation in Nelder-Mead
  //   algorithm", Kelley uses the constant value of 1.e-4.
  // * in the 1999 book "Iterative Methods for Optimization", Kelley uses normalization.
  // Results are slightly changed, as indicated in the book/paper (the modification is 
  // not mentioned, but the iteration number when the restart is performed
  // is modified).
  newobj.kelleynormalizationflag = 1
  // The current value of Kelley's alpha, after normalization, if required
  newobj.kelleyalpha = 1.e-4;
  // The optimum simplex, after one optimization process
  newobj.simplexopt = [];
  // The maximum number of restarts
  newobj.restartmax = 3;
  // The epsilon value for O'Neill restart detection
  newobj.restarteps = %eps;
  // The step length for O'Neill restart detection
  newobj.restartstep = 1.0;
  // Possible values : "oriented", "axes", "spendley", "pfeffer" 
  newobj.restartsimplexmethod = "oriented";
  // Possible values : 0/1
  newobj.restartflag = 0;
  // Number of restarts performed
  newobj.restartnb = 0;
  // Type of restart detection method : "kelley", "oneill"
  newobj.restartdetection = "oneill";
  // Set to 1 when the startup has been performed
  newobj.startupflag = 0;
  // Initial size of the simplex, for the tolerance on the simplex size
  newobj.simplexsize0 = 0.0
  // Number of points required in the simplex (for Box method)
  newobj.boxnbpoints = "2n"
  // Effective number of points required in the simplex (for Box method)
  newobj.boxnbpointseff = 0
  // Number of loops performed to satisfy nonlinear
  // inequality constraints (for Box method)
  newobj.nbineqloops = 10
  // The scaling coefficient in nonlinear inequality constraints
  // in Box method, in (0,1) range
  newobj.ineqscaling = 0.5
  // Set to 1 if the cost function is vectorized
  newobj.vectorizefunction = 0
endfunction

