// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
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
//
function myoutputcmd ( state , data )
  global _OUTPUCMDFLAG_
  // Unload the array, just to make sure that the minimum is there
  iter = data.iteration
  fc = data.funccount
  fval = data.fval
  x = data.x
  _OUTPUCMDFLAG_ = 1
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
//
function myoutputcmd2 ( state , data , myobj )
  global _OUTPUCMDFLAG_
  // Unload the array, just to make sure that the minimum is there
  iter = data.iteration
  fc   = data.funccount
  fval = data.fval
  x    = data.x
  _OUTPUCMDFLAG_ = myobj.myarg
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
function [ y , index , mydata ] = rosenbrock2 ( x , index , mydata )
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
assert_equal ( nbvar , 2 );
// Check cost function without additionnal argument
opt = optimbase_configure(opt,"-function",rosenbrock);
[this,f , index ] = optimbase_function ( opt , [0.0 0.0] , 2 );
assert_close ( f , 1.0 , %eps );
// Check cost function with additionnal argument
opt = optimbase_configure(opt,"-function",rosenbrock2);
opt = optimbase_configure(opt,"-costfargument",mystuff);
[this,f, index ] = optimbase_function ( opt , [0.0 0.0] , 2 );
assert_close ( f , 144.0 , %eps );
// Check initial guess
opt = optimbase_configure(opt,"-x0",[-1.2 1.0]');
x0 = optimbase_cget(opt,"-x0");
assert_close ( x0 , [-1.2 1.0]' , %eps);
// Check maxiter
opt = optimbase_configure(opt,"-maxiter",200);
maxiter = optimbase_cget(opt,"-maxiter");
assert_equal ( maxiter , 200);
// Check maxfunevals
opt = optimbase_configure(opt,"-maxfunevals",200);
maxfunevals = optimbase_cget(opt,"-maxfunevals");
assert_equal ( maxfunevals , 200);
// Check tolfunrelative
opt = optimbase_configure(opt,"-tolfunrelative",10*%eps);
tolfunrelative = optimbase_cget(opt,"-tolfunrelative");
assert_equal ( tolfunrelative , 10*%eps );
// Check tolxrelative
opt = optimbase_configure(opt,"-tolxrelative",10*%eps);
tolxrelative = optimbase_cget(opt,"-tolxrelative");
assert_equal ( tolxrelative , 10*%eps );
// Check verbose
opt = optimbase_configure(opt,"-verbose",1);
verbose = optimbase_cget(opt,"-verbose");
assert_equal ( verbose , 1 );
opt = optimbase_configure(opt,"-verbose",0);
// Check verbose termination
opt = optimbase_configure(opt,"-verbosetermination",1);
verbosetermination = optimbase_cget(opt,"-verbosetermination");
assert_equal ( verbosetermination , 1 );
opt = optimbase_configure(opt,"-verbosetermination",0);
// Check optimum
opt = optimbase_set(opt,"-xopt",[1.0 1.0]);
xopt = optimbase_get(opt,"-xopt");
assert_close ( xopt , [1.0 1.0], %eps );
// Check function value at optimum
opt = optimbase_set(opt,"-fopt",1.0);
fopt = optimbase_get(opt,"-fopt");
assert_close ( fopt , 1.0 , %eps );
// Check status
opt = optimbase_set(opt,"-status","maxiter");
status = optimbase_get(opt,"-status");
assert_equal ( status , "maxiter" );
// Log a message
opt = optimbase_configure(opt,"-verbose",1);
opt = optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
// Log a message relative to the stopping rule
opt = optimbase_configure(opt,"-verbosetermination",1);
opt = optimbase_stoplog ( opt , "My interesting stop message" );
opt = optimbase_configure(opt,"-verbosetermination",0);
// Check output command without additionnal argument
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
assert_equal ( _OUTPUCMDFLAG_ , 1 );
// Check output command with additionnal argument
opt = optimbase_configure(opt,"-outputcommand",myoutputcmd2);
opt = optimbase_configure(opt,"-outputcommandarg",myobj);
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
assert_equal ( _OUTPUCMDFLAG_ , 12. );
// Check incriter
opt = optimbase_incriter ( opt );
iter = optimbase_get ( opt , "-iterations");
assert_equal ( iter , 1 );
// Check history storing with xopt
opt = optimbase_configure ( opt , "-storehistory" , 1 );
opt = optimbase_histset ( opt , 1 , "-xopt" , [1.0 1.0]' );
x0 = optimbase_histget ( opt , 1 , "-xopt" );
assert_close ( x0 , [1.0 1.0]', %eps );
// Check history storing with fopt
opt = optimbase_configure ( opt , "-storehistory" , 1 );
opt = optimbase_histset ( opt , 1 , "-fopt" , 1.0 );
f0 = optimbase_histget ( opt , 1 , "-fopt" );
assert_close ( f0 , 1.0, %eps );
// Check display
optimbase_display ( opt );
// Check the boundsmin, boundsmax and nbineqconst
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
boundsmin = optimbase_cget ( opt , "-boundsmin" );
assert_equal ( boundsmin , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
boundsmax = optimbase_cget ( opt , "-boundsmax" );
assert_equal ( boundsmax , [5.0 5.0] );
opt = optimbase_configure ( opt , "-nbineqconst" , 3 );
nbineqconst = optimbase_cget ( opt , "-nbineqconst" );
assert_equal ( nbineqconst , 3 );
// Cleanup
opt = optimbase_destroy(opt);

//
// Test error cases
//
opt = optimbase_new ();
//
// Test wrong initial guess
//
cmd = "optimbase_configure(opt,''-x0'',[-1.2 1.0])";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_configure: The x0 vector is expected to be a column matrix, but current shape is 1 x 2";
assert_equal ( computed , expected );
//
// Test wrong -tolxmethod
//
cmd = "optimbase_configure(opt,''-tolxmethod'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_configure: Unknown value foo for -tolxmethod option";
assert_equal ( computed , expected );
//
// Test wrong -tolfunmethod
//
cmd = "optimbase_configure(opt,''-tolfunmethod'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_configure: Unknown value foo for -tolfunmethod";
assert_equal ( computed , expected );
// Cleanup
opt = optimbase_destroy(opt);

//
// Test outstruct when no -outputcommand is defined
//
opt = optimbase_new ();
cmd = "data = optimbase_outstruct ( opt )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_outstruct: No output command is defined.";
assert_equal ( computed , expected );
// Cleanup
opt = optimbase_destroy(opt);


//
// Test optimbase_cget with unknown key
//
opt = optimbase_new ();
cmd = "value = optimbase_cget (opt,''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_cget: Unknown key foo";
assert_equal ( computed , expected );
// Cleanup
opt = optimbase_destroy(opt);

//
// Test optimbase_get with unknown key
//
opt = optimbase_new ();
cmd = "value = optimbase_get (opt,''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_get: Unknown key foo";
assert_equal ( computed , expected );
// Cleanup
opt = optimbase_destroy(opt);

//
// Test various errors 
//
opt = optimbase_new ();
// Test -historyxopt when there is no history
cmd = "value = optimbase_get (opt,''-historyxopt'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_get: History disabled ; enable -storehistory option.";
assert_equal ( computed , expected );
// Test -historyfopt when there is no history
cmd = "value = optimbase_get (opt,''-historyfopt'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_get: History disabled ; enable -storehistory option.";
assert_equal ( computed , expected );
// Test optimbase_function when there is no function
cmd = "[ opt , f , index ] = optimbase_function ( opt , [] , 1 )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_function: Empty function (use -function option).";
assert_equal ( computed , expected );
// Test optimbase_histget ( this , iter , key ) when there is no history
cmd = "optimbase_histget ( opt , 1 , ''-xopt'' )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_histget: History disabled ; turn on -storehistory option.";
assert_equal ( computed , expected );
// Test optimbase_histget ( this , iter , key ) with negative iteration
opt = optimbase_configure ( opt , "-storehistory" , 1 );
cmd = "optimbase_histget ( opt , -1 , ''-xopt'' )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_histget: Negative iteration index -1 is not allowed.";
assert_equal ( computed , expected );
// Cleanup
opt = optimbase_destroy(opt);


