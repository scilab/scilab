// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// myoutputcmd --
//  This command is called back by the Optimization
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
// stop : set to %f to interrupt algorithm
//
function stop = myoutputcmd ( state , data )
    global _OUTPUCMDFLAG_
    // Unload the array, just to make sure that the minimum is there
    iter = data.iteration
    fc = data.funccount
    fval = data.fval
    x = data.x
    _OUTPUCMDFLAG_ = 1
    stop = %f
endfunction

global _OUTPUCMDFLAG_
_OUTPUCMDFLAG_ = 0

//
// myoutputcmd2 --
//  This command is called back by the Optimization
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//  myobj : a user-defined data structure
// stop : set to %f to interrupt algorithm
//
function stop = myoutputcmd2 ( state , data , myobj )
    global _OUTPUCMDFLAG_
    // Unload the array, just to make sure that the minimum is there
    iter = data.iteration
    fc   = data.funccount
    fval = data.fval
    x    = data.x
    _OUTPUCMDFLAG_ = myobj.myarg
    stop = %f
endfunction


global _OUTPUCMDFLAG_
_OUTPUCMDFLAG_ = 0

myobj = tlist(["T_MYSTUFF","myarg"]);
myobj.myarg = 12;

//
// In this case, the mydata variable is passed
// explicitely by the optimization class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ y , index ] = rosenbrock2 ( x , index , mydata )
    a = mydata.a
    y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
endfunction

//
// Test with an additional argument
//
mystuff = tlist(["T_MYSTUFF","a"]);
mystuff.a = 12.0;

//
// Test nearly all features of the optimization "abstract" class
//

opt = optimbase_new ();
// Check number of variables
opt = optimbase_configure(opt,"-numberofvariables",2);
nbvar = optimbase_cget(opt,"-numberofvariables");
assert_checkequal ( nbvar , 2 );
// Check cost function without additional argument
opt = optimbase_configure(opt,"-function",rosenbrock);
[this,f , index ] = optimbase_function ( opt , [0.0 0.0] , 2 );
assert_checkalmostequal ( f , 1.0 , %eps );
// Check cost function with additional argument
opt = optimbase_configure(opt,"-function",list(rosenbrock2,mystuff));
[this,f, index ] = optimbase_function ( opt , [0.0 0.0] , 2 );
assert_checkalmostequal ( f , 144.0 , %eps );
// Check initial guess
opt = optimbase_configure(opt,"-x0",[-1.2 1.0]');
x0 = optimbase_cget(opt,"-x0");
assert_checkalmostequal ( x0 , [-1.2 1.0]' , %eps);
// Check maxiter
opt = optimbase_configure(opt,"-maxiter",200);
maxiter = optimbase_cget(opt,"-maxiter");
assert_checkequal ( maxiter , 200);
// Check maxfunevals
opt = optimbase_configure(opt,"-maxfunevals",200);
maxfunevals = optimbase_cget(opt,"-maxfunevals");
assert_checkequal ( maxfunevals , 200);
// Check tolfunrelative
opt = optimbase_configure(opt,"-tolfunrelative",10*%eps);
tolfunrelative = optimbase_cget(opt,"-tolfunrelative");
assert_checkequal ( tolfunrelative , 10*%eps );
// Check tolxrelative
opt = optimbase_configure(opt,"-tolxrelative",10*%eps);
tolxrelative = optimbase_cget(opt,"-tolxrelative");
assert_checkequal ( tolxrelative , 10*%eps );
// Check verbose
opt = optimbase_configure(opt,"-verbose",1);
verbose = optimbase_cget(opt,"-verbose");
assert_checkequal ( verbose , 1 );
opt = optimbase_configure(opt,"-verbose",0);
// Check verbose termination
opt = optimbase_configure(opt,"-verbosetermination",1);
verbosetermination = optimbase_cget(opt,"-verbosetermination");
assert_checkequal ( verbosetermination , 1 );
opt = optimbase_configure(opt,"-verbosetermination",0);
// Check optimum
opt = optimbase_set(opt,"-xopt",[1.0 1.0]);
xopt = optimbase_get(opt,"-xopt");
assert_checkalmostequal ( xopt , [1.0 1.0], %eps );
// Check function value at optimum
opt = optimbase_set(opt,"-fopt",1.0);
fopt = optimbase_get(opt,"-fopt");
assert_checkalmostequal ( fopt , 1.0 , %eps );
// Check status
opt = optimbase_set(opt,"-status","maxiter");
status = optimbase_get(opt,"-status");
assert_checkequal ( status , "maxiter" );
// Log a message
opt = optimbase_configure(opt,"-verbose",1);
opt = optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
// Log a message relative to the stopping rule
opt = optimbase_configure(opt,"-verbosetermination",1);
opt = optimbase_stoplog ( opt , "My interesting stop message" );
opt = optimbase_configure(opt,"-verbosetermination",0);
// Check output command without additional argument
opt = optimbase_configure(opt,"-outputcommand",myoutputcmd);
brutedata = optimbase_outstruct ( opt );
mydata = tlist(["T_MYDATA",...
"x","fval","iteration","funccount",...
"myspecialdata"]);
mydata.x = brutedata.x;
mydata.fval = brutedata.fval;
mydata.iteration = brutedata.iteration;
mydata.funccount = brutedata.funccount;
mydata.myspecialdata = "yahoo !";
optimbase_outputcmd ( opt , "init" , mydata );
assert_checkequal ( _OUTPUCMDFLAG_ , 1 );
// Check output command with additional argument
opt = optimbase_configure(opt,"-outputcommand",list(myoutputcmd2,myobj));
brutedata = optimbase_outstruct ( opt );
mydata = tlist(["T_MYDATA",...
"x","fval","iteration","funccount",...
"myspecialdata"]);
mydata.x = brutedata.x;
mydata.fval = brutedata.fval;
mydata.iteration = brutedata.iteration;
mydata.funccount = brutedata.funccount;
mydata.myspecialdata = "yahoo !";
optimbase_outputcmd ( opt , "init" , mydata );
assert_checkequal ( _OUTPUCMDFLAG_ , 12. );
// Check incriter
opt = optimbase_incriter ( opt );
iter = optimbase_get ( opt , "-iterations");
assert_checkequal ( iter , 1 );
// Check history storing with xopt
opt = optimbase_configure ( opt , "-storehistory" , %t );
opt = optimbase_histset ( opt , 1 , "-xopt" , [1.0 1.0]' );
x0 = optimbase_histget ( opt , 1 , "-xopt" );
assert_checkalmostequal ( x0 , [1.0 1.0]', %eps );
// Check history storing with fopt
opt = optimbase_configure ( opt , "-storehistory" , %t );
opt = optimbase_histset ( opt , 1 , "-fopt" , 1.0 );
f0 = optimbase_histget ( opt , 1 , "-fopt" );
assert_checkalmostequal ( f0 , 1.0, %eps );
// Check the boundsmin, boundsmax and nbineqconst
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
boundsmin = optimbase_cget ( opt , "-boundsmin" );
assert_checkequal ( boundsmin , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
boundsmax = optimbase_cget ( opt , "-boundsmax" );
assert_checkequal ( boundsmax , [5.0 5.0] );
opt = optimbase_configure ( opt , "-nbineqconst" , 3 );
nbineqconst = optimbase_cget ( opt , "-nbineqconst" );
assert_checkequal ( nbineqconst , 3 );
// Cleanup
opt = optimbase_destroy(opt);

//
// Test error cases
//
opt = optimbase_new ();
//
// Test wrong initial guess
//
cmd = "optimbase_configure(opt,''-x0'',[-1.2 1.0; -1.0 1.2])";
assert_checkerror(cmd,"%s: Wrong size for x0 argument: A vector expected.", [], "optimbase_configure");
//
// Test wrong -tolxmethod
//
cmd = "optimbase_configure(opt,''-tolxmethod'',''foo'')";
assert_checkerror(cmd,"%s: Expected boolean but for variable %s at input #%d, got %s instead.",[],"optimbase_typeboolean","value",3,"string");
//
// Test wrong -tolfunmethod
//
cmd = "optimbase_configure(opt,''-tolfunmethod'',''foo'')";
assert_checkerror(cmd,"%s: Expected boolean but for variable %s at input #%d, got %s instead.",[],"optimbase_typeboolean","value",3,"string");
opt = optimbase_destroy(opt);

//
// Test outstruct when no -outputcommand is defined
//
opt = optimbase_new ();
cmd = "data = optimbase_outstruct ( opt )";
assert_checkerror(cmd,"%s: No output command is defined.",[], "optimbase_outstruct");
opt = optimbase_destroy(opt);

//
// Test optimbase_cget with unknown key
//
opt = optimbase_new ();
cmd = "value = optimbase_cget (opt,''foo'')";
assert_checkerror(cmd,"%s: Unknown key %s",[],"optimbase_cget" , "foo");
opt = optimbase_destroy(opt);

//
// Test optimbase_get with unknown key
//
opt = optimbase_new ();
cmd = "value = optimbase_get (opt,''foo'')";
assert_checkerror(cmd,"%s: Unknown key %s",[],"optimbase_get" , "foo");
opt = optimbase_destroy(opt);

//
// Test various errors
//
opt = optimbase_new ();
// Test -historyxopt when there is no history
cmd = "value = optimbase_get (opt,''-historyxopt'')";
assert_checkerror(cmd,"%s: History disabled ; enable -storehistory option.",[],"optimbase_get");
// Test -historyfopt when there is no history
cmd = "value = optimbase_get (opt,''-historyfopt'')";
assert_checkerror(cmd,"%s: History disabled ; enable -storehistory option.",[],"optimbase_get");
// Test optimbase_function when there is no function
cmd = "[ opt , f , index ] = optimbase_function ( opt , [] , %t )";
assert_checkerror(cmd,"%s: Empty function (use -function option).",[],"optimbase_function");
// Test optimbase_histget ( this , iter , key ) when there is no history
cmd = "optimbase_histget ( opt , 1 , ''-xopt'' )";
assert_checkerror(cmd,"%s: History disabled ; turn on -storehistory option.",[],"optimbase_histget");
// Test optimbase_histget ( this , iter , key ) with negative iteration
opt = optimbase_configure ( opt , "-storehistory" , %t );
cmd = "optimbase_histget ( opt , -1 , ''-xopt'' )";
assert_checkerror(cmd,"%s: Negative iteration index %d is not allowed.",[],"optimbase_histget",-1);
// Cleanup
opt = optimbase_destroy(opt);

//
// Test the printing system
//
opt = optimbase_new ();
string(opt)
opt
opt = optimbase_destroy(opt);

