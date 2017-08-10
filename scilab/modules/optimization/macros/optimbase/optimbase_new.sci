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
// optimbase_new --
//   Creates a new Optimization object.
//
function newobj = optimbase_new ()
    [lhs,rhs]=argn();
    if ( rhs <> 0 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_new", 0);
        error(errmsg)
    end
    newobj = tlist(["TOPTIM"
    "verbose"
    "x0"
    "fx0"
    "xopt"
    "fopt"
    "tolfunabsolute"
    "tolfunrelative"
    "tolfunmethod"
    "tolxabsolute"
    "tolxrelative"
    "tolxmethod"
    "funevals"
    "maxfunevals"
    "iterations"
    "maxiter"
    "fun"
    "status"
    "historyfopt"
    "historyxopt"
    "verbosetermination"
    "outputcommand"
    "numberofvariables"
    "storehistory"
    "boundsmin"
    "boundsmax"
    "nbineqconst"
    "logfile"
    "logfilehandle"
    "logstartup"
    "withderivatives"
    //
    // Obsolete options.
    //
    "costfargument"
    "outputcommandarg"
    ]);
    // The number of variables to optimize
    newobj.numberofvariables = 0
    // The verbose option, controlling the amount of messages
    newobj.verbose = 0;
    // The verbose option for termination criteria
    newobj.verbosetermination = 0;
    // The initial guess
    newobj.x0 = [];
    // The value of the function for the initial guess
    newobj.fx0 = [];
    // The maximum number of function evaluations
    newobj.maxfunevals = 100;
    // The maximum number of iterations
    newobj.maxiter = 100;
    // Possible values : %f, %t
    // This criteria is suitable for functions which minimum is
    // associated with a function value equal to 0.
    newobj.tolfunmethod = %f;
    // The absolute tolerance on function value
    newobj.tolfunabsolute = 0.0;
    // The relative tolerance on function value
    newobj.tolfunrelative = %eps;
    // The absolute tolerance on x
    newobj.tolxabsolute = 0.0;
    // The relative tolerance on x
    newobj.tolxrelative = sqrt(%eps);
    // The number of function evaluations
    newobj.funevals = 0;
    // The number of iterations
    newobj.iterations = 0;
    // The optimum parameter
    newobj.xopt = 0;
    // The optimum function value
    newobj.fopt = 0;
    // Possible values : %f, %t
    newobj.tolxmethod = %t;
    // The status of the optimization
    newobj.status = "";
    // The command called back for output
    newobj.outputcommand = "";
    // The cost function
    newobj.fun = "";
    // The flag which enables/disables the storing of the history
    newobj.storehistory = %f;
    // The array to store the history for xopt
    newobj.historyxopt = list();
    // The array to store the history for fopt
    newobj.historyfopt = [];
    // Maximum bounds for the parameters
    newobj.boundsmax = [];
    // Minimum bounds for the parameters
    newobj.boundsmin = [];
    // The number of nonlinear inequality constraints
    newobj.nbineqconst = 0;
    // The name of the log file
    newobj.logfile = "";
    // The handle for the log file
    newobj.logfilehandle = 0;
    // Set to %t when the logging is started up
    newobj.logstartup = %f;
    // Set to %t when the method uses derivatives
    newobj.withderivatives = %f
    //
    // Obsolete options
    //
    // The costf argument is initialized as a string.
    // If the user configure this option, it is expected
    // that a matrix of values or a list, tlist, mlist is
    // passed so that the argument is appended to the name of the
    // function.
    newobj.costfargument = "";
    // The outputcommand argument is initialized as a string.
    // If the user configure this option, it is expected
    // that a matrix of values or a list, tlist, mlist is
    // passed so that the argument is appended to the name of the
    // function.
    newobj.outputcommandarg = ""
endfunction

