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
// Check behaviour with default settings.
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0].');
nm = nmplot_configure(nm,"-maxiter",10);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","axes");
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
//nm = nmplot_configure(nm,"-verbose",1);
nm = nmplot_configure(nm,"-verbosetermination",0);
nm = nmplot_configure(nm,"-simplexfn","rosenbrock.history.simplex.txt");
nm = nmplot_configure(nm,"-fbarfn","rosenbrock.history.fbar.txt");
nm = nmplot_configure(nm,"-foptfn","rosenbrock.history.fopt.txt");
nm = nmplot_configure(nm,"-sigmafn","rosenbrock.history.sigma.txt");
sfn = nmplot_cget ( nm , "-simplexfn" );
assert_equal ( sfn , "rosenbrock.history.simplex.txt" );
sfn = nmplot_cget ( nm , "-fbarfn" );
assert_equal ( sfn , "rosenbrock.history.fbar.txt" );
sfn = nmplot_cget ( nm , "-foptfn" );
assert_equal ( sfn , "rosenbrock.history.fopt.txt" );
sfn = nmplot_cget ( nm , "-sigmafn" );
assert_equal ( sfn , "rosenbrock.history.sigma.txt" );
// Check the display
nmplot_display(nm);
nm = nmplot_destroy(nm);

