// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// neldermead_new --
//   Creates a new Nelder-Mead object.
//
function newobj = neldermead_new ()
    newobj = tlist(["TNELDER"
    "optbase"
    "method"
    "simplex0"
    "simplex0method"
    "simplex0length"
    "rho"
    "chi"
    "gamma"
    "sigma"
    "tolfstdeviation"
    "tolfstdeviationmethod"
    "tolsimplexizeabsolute"
    "tolsimplexizerelative"
    "tolsimplexizemethod"
    "simplexsize0"
    "toldeltafv"
    "tolssizedeltafvmethod"
    "historysimplex"
    "coords0"
    "simplex0deltausual"
    "simplex0deltazero"
    "restartsimplexmethod"
    "simplexopt"
    "restartmax"
    "restarteps"
    "restartstep"
    "kelleystagnationflag"
    "kelleynormalizationflag"
    "kelleystagnationalpha0"
    "kelleyalpha"
    "restartnb"
    "restartflag"
    "restartdetection"
    "startupflag"
    "boxnbpoints"
    "boxnbpointseff"
    "boxineqscaling"
    "checkcostfunction"
    "scalingsimplex0"
    "guinalphamin"
    "boxboundsalpha"
    "boxtermination"
    "boxtolf"
    "boxnbmatch"
    "boxkount"
    "boxreflect"
    "tolvarianceflag"
    "tolabsolutevariance"
    "tolrelativevariance"
    "variancesimplex0"
    "mymethod"
    "greedy"
    //
    // Obsolete options
    //
    "myterminate"
    "myterminateflag"
    ]);

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
    // Possible values : %t, %f
    newobj.tolfstdeviationmethod = %f;
    // The absolute tolerance for the simplex size
    newobj.tolsimplexizeabsolute = 0.0;
    // The relative tolerance for the simplex size
    newobj.tolsimplexizerelative = %eps;
    // Possible values : %t, %f
    // Note :
    //   If the simplex method converges, the simplex size is near zero.
    newobj.tolsimplexizemethod = %t;
    // The tolerance for the function value delta
    newobj.toldeltafv = %eps;
    // Possible values : %t, %f
    newobj.tolssizedeltafvmethod = %f;
    // The value used in Pfeffer method initial simplex computation for non-zero parameters
    newobj.simplex0deltausual = 0.05;
    // The value used in Pfeffer method initial simplex computation for zero parameters
    newobj.simplex0deltazero = 0.0075;
    // The coordinates of the initial simplex, given by the user
    newobj.coords0 = [];
    // Initialize the simplex history
    newobj.historysimplex = [];
    // The Kelley stagnation detection in termination criteria :  0/1
    // (i.e. sufficient decrease of function value)
    newobj.kelleystagnationflag = %f
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
    newobj.kelleynormalizationflag = %t
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
    // Possible values : %t, %f
    newobj.restartflag = %f;
    // Number of restarts performed
    newobj.restartnb = 0;
    // Type of restart detection method : "kelley", "oneill"
    newobj.restartdetection = "oneill";
    // Set to %t when the startup has been performed
    newobj.startupflag = %f;
    // Initial size of the simplex, for the tolerance on the simplex size
    newobj.simplexsize0 = 0.0
    // Number of points required in the simplex (for Box method)
    newobj.boxnbpoints = "2n"
    // Effective number of points required in the simplex (for Box method)
    newobj.boxnbpointseff = 0
    // The scaling coefficient in nonlinear inequality constraints
    // in Box method, in (0,1) range
    newobj.boxineqscaling = 0.5
    // Set to %f to disable the checking of the connection of the cost function
    newobj.checkcostfunction = %t;
    // The scaling algorithm : "tox0", "tocentroid"
    newobj.scalingsimplex0 = "tox0";
    // Minimum alpha for constraints scaling
    newobj.guinalphamin = 1.e-5;
    // Box's alpha coefficient for bounds constraints.
    // The value used in Box's paper was 1.e-6 (delta in
    // Richardson and Kuester's algorithm 454)
    newobj.boxboundsalpha = 1.e-6
    // Set to 1 to enable Box termination criteria
    newobj.boxtermination = %f
    // The absolute tolerance on function value in Box termination criteria (beta in
    // Richardson and Kuester's algorithm 454)
    newobj.boxtolf = 1.e-5
    // The number of consecutive match in Box termination criteria (gamma in
    // Richardson and Kuester's algorithm 454)
    newobj.boxnbmatch = 5
    // Current number of consecutive match
    newobj.boxkount = 0
    // Box reflection/expansion factor
    newobj.boxreflect = 1.3
    // Set to %t to enable tolerance on variance
    newobj.tolvarianceflag = %f;
    // Absolute tolerance on variance
    newobj.tolabsolutevariance = 0.0;
    // Relative tolerance on variance
    newobj.tolrelativevariance = %eps;
    // The variance of the initial simplex
    newobj.variancesimplex0 = 0.0;
    // User-defined algorithm
    newobj.mymethod = []
    // Set to %t to enable greedy Nelder-Mead
    newobj.greedy = %f;
    //
    // Obsolete options
    //
    // User-defined terimination criteria
    newobj.myterminate = []
    // Flag to enable the user-defined terimination criteria
    newobj.myterminateflag = %f

endfunction

