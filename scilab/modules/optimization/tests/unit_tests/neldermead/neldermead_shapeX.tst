// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->


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

// Checks that the shape of x is always the same,
// i.e. a column vector.
function [ f , index ] = optimtestcase ( x , index )
  if ( size ( x ) <> [4 1] ) then
    error ( "Wrong shape for x !" )
  end
  f = x.' * x
endfunction


//
// Test search with variable algorithm
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[1 2 3 4]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);

//
// Test search with fixed algorithm
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[1 2 3 4]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);

//
// Test with non linear constraints 
//
function [ f , c , index ] = optimtestcase2 ( x , index )
  f = []
  c = []
  disp(x)
  if ( size ( x ) <> [4 1] ) then
    error ( "Wrong shape for x !" )
  end
  if ( ( index == 2 ) | ( index == 6 ) ) then
      f = x.' * x
  end
  if ( ( index == 5 ) | ( index == 6 ) ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0 0 0 0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);

