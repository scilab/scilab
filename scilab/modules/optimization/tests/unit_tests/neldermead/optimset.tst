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

// 
// Test #1 : Without parameters 
//
op = optimset ();
if op.Display <> [] then pause,end
if op.FunValCheck <> [] then pause,end
if op.MaxFunEvals <> [] then pause,end
if op.MaxIter <> [] then pause,end
if op.OutputFcn <> [] then pause,end
if op.PlotFcns <> [] then pause,end
if op.TolFun <> [] then pause,end
if op.TolX <> [] then pause,end
clear op
function y = myoutputfun (x)
  y = x;
endfunction
function y = myplotfun (x)
  y = x;
endfunction
// 
// Test #2 : With parameters
//
op = optimset (...
  'Display','iter',...
  "FunValCheck","on",...
  "MaxFunEvals",100,...
  "MaxIter",110,...
  "OutputFcn",myoutputfun,...
  "PlotFcns",myplotfun,...
  "TolFun",1.e-12,...
  "TolX",1.e-13...
  );
if op.Display <> "iter" then pause,end
if op.FunValCheck <> "on" then pause,end
if op.MaxFunEvals <> 100 then pause,end
if op.MaxIter <> 110 then pause,end
//if op.OutputFcn <> myoutputfun then pause,end
//if op.PlotFcns <> myplotfun then pause,end
if op.TolFun <> 1.e-12 then pause,end
if op.TolX <> 1.e-13 then pause,end
clear op
// 
// Test #3 : Copy one option set into another
// 
op1 = optimset ("TolFun",1.e-12);
op2 = optimset (op1,"TolX",1.e-13);
if op2.TolFun <> 1.e-12 then pause,end
if op2.TolX <> 1.e-13 then pause,end
clear op1
clear op2
// 
// Test #3 : with one method name
// 
op = optimset ("fminsearch");
if op.TolFun <> 1.e-4 then pause,end
if op.TolX <> 1.e-4 then pause,end
if op.Display <> "notify" then pause,end
if op.MaxFunEvals <> "200*numberofvariables" then pause,end
if op.MaxIter <> "200*numberofvariables" then pause,end
clear op

//
// Test where the first input argument is not a struct
//
cmd = "optimset (''foo'',''MaxFunEvals'',100)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimset: Odd number of arguments : the first argument is expected to be a struct, but is a string";
assert_equal ( computed , expected );
//
// Test where the key is unknown
//
cmd = "optimset (''foo'',100)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimset: Unrecognized parameter name ''foo''.";
assert_equal ( computed , expected );

//
// Test where the algorithm is unknown
//
cmd = "optimset (''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimset: No default options available: the function ''foo'' does not exist on the path.";
assert_equal ( computed , expected );

