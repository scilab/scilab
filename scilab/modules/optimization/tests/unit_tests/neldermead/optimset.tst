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
assert_equal ( op.Display , [] );
assert_equal ( op.FunValCheck , [] );
assert_equal ( op.MaxFunEvals , [] );
assert_equal ( op.MaxIter , [] );
assert_equal ( op.OutputFcn , [] );
assert_equal ( op.PlotFcns , [] );
assert_equal ( op.TolFun , [] );
assert_equal ( op.TolX , [] );
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
assert_equal ( op.Display , "iter" );
assert_equal ( op.FunValCheck , "on" );
assert_equal ( op.MaxFunEvals , 100 );
assert_equal ( op.MaxIter , 110 );
//assert_equal ( op.OutputFcn , myoutputfun );
//assert_equal ( op.PlotFcns , myplotfun );
assert_equal ( op.TolFun , 1.e-12 );
assert_equal ( op.TolX , 1.e-13 );
clear op
// 
// Test #3 : Copy one option set into another
// 
op1 = optimset ("TolFun",1.e-12);
op2 = optimset (op1,"TolX",1.e-13);
assert_equal ( op2.TolFun , 1.e-12 );
assert_equal ( op2.TolX , 1.e-13 );
clear op1
clear op2
// 
// Test #3 : with one method name
// 
op = optimset ("fminsearch");
assert_equal ( op.TolFun , 1.e-4 );
assert_equal ( op.TolX , 1.e-4 );
assert_equal ( op.Display , "notify" );
assert_equal ( op.MaxFunEvals , "200*numberofvariables" );
assert_equal ( op.MaxIter , "200*numberofvariables" );
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
//
// Test where the Display key is unknown
//
cmd = "optimset (''Display'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimset: Unrecognized value ''foo'' for ''Display'' option.";
assert_equal ( computed , expected );
//
// Test all possible values of Display
//
op = optimset ( "Display" , "final" );
assert_equal ( op.Display , "final" );
op = optimset ( "Display" , "iter" );
assert_equal ( op.Display , "iter" );
op = optimset ( "Display" , "off" );
assert_equal ( op.Display , "off" );
op = optimset ( "Display" , "notify" );
assert_equal ( op.Display , "notify" );

