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
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
//
function [ f , c , index ] = optimtestcase ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction


//
// Test search with various error cases
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-4);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-simplex0length",20.0);
//
// Test with inconsistent bounds
//
nm = neldermead_configure(nm,"-boundsmin",[10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[-10.0 10.0 10.0 10.0]);
cmd = "nm = neldermead_search(nm)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_startup: The max bound -1.000000e+001 for variable #1 is lower than the min bound 1.000000e+001.";
assert_equal ( computed , expected );
//
// Test with wrong number of min bounds
//
nm = neldermead_configure(nm,"-boundsmin",[10.0]);
nm = neldermead_configure(nm,"-boundsmax",[-10.0 10.0 10.0 10.0]);
cmd = "nm = neldermead_search(nm)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_startup: The number of variables 4 does not match the number of min bounds 1 from [10]";
assert_equal ( computed , expected );
//
// Test with wrong number of max bounds
//
nm = neldermead_configure(nm,"-boundsmin",[10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[-10.0]);
cmd = "nm = neldermead_search(nm)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_startup: The number of variables 4 does not match the number of max bounds 1 from [-10]";
assert_equal ( computed , expected );
//
// Test with Box algorithm and randomized bounds simplex and no bounds
//
nm = neldermead_configure(nm,"-boundsmin",[]);
nm = neldermead_configure(nm,"-boundsmax",[]);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
cmd = "nm = neldermead_search(nm)";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_startup: Randomized bounds initial simplex is not available without bounds.";
assert_equal ( computed , expected );
//
// Clean-up
//
nm = neldermead_destroy(nm);


//
// Test search with verbose to log file
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-logfile" , "search.txt" );
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);
computed = deletefile("search.txt");
assert_equal ( computed , %t );


