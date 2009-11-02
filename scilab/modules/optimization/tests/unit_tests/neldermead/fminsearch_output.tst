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
// Use output function
//
// outfun --
//   A sample output function
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funccount : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
//
function outfun ( x , optimValues , state )
  plot( x(1),x(2),'.');
  // Unload all fields and check consistent values
  fc = optimValues.funccount;
  fv = optimValues.fval;
  it = optimValues.iteration;
  pr = optimValues.procedure;
  select pr
  case "initial simplex"
    // OK
  case "expand"
    // OK
  case "reflect"
    // OK
  case "shrink"
    // OK
  case "contract inside"
    // OK
  case "contract outside"
    // OK
  case ""
    // OK
  else
    error ( sprintf ( "Unknown procedure %s." , pr ) )
  end
  select state
  case "init"
    // OK
  case "iter"
    // OK
  case "done"
    // OK
  else
    error ( sprintf ( "Unknown state %s." , state ) )
  end
  mprintf ( "%d %e %d -%s- %s\n" , fc , fv , it , pr , state )
endfunction
opt = optimset ( "OutputFcn" , outfun);
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use several output functions
//
function outfun2 ( x , optimValues , state )
  scf ( fig1 );
  plot( x(1),x(2),'.');
endfunction
function outfun3 ( x , optimValues , state )
  scf ( fig2 );
  plot( x(1),x(2),'o');
endfunction
myfunctions = list ( outfun2 , outfun3 );
fig1 = scf(1000);
fig2 = scf(1001);
opt = optimset ( "OutputFcn" , myfunctions );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(fig1);
close(fig2);
//
// Use plot function
//
//
// plotfun --
//   A sample plot function
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funcCount" : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
//
function plotfun ( x , optimValues , state )
  plot(x(1),x(2),'.');
endfunction
opt = optimset ( "PlotFcns" , plotfun);
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use several plot functions
//
function plotfun2 ( x , optimValues , state )
  scf ( fig1 );
  plot( x(1),x(2),'.');
endfunction
function plotfun3 ( x , optimValues , state )
  scf ( fig2 );
  plot( x(1),x(2),'o');
endfunction
myfunctions = list ( plotfun2 , plotfun3 );
fig1 = scf(1000);
fig2 = scf(1001);
opt = optimset ( "PlotFcns" , myfunctions );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(fig1);
close(fig2);
//
// Use optimplotfval plot function
//
opt = optimset ( "PlotFcns" , optimplotfval );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use optimplotx plot function
//
opt = optimset ( "PlotFcns" , optimplotx );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use optimplotfunccount plot function
//
opt = optimset ( "PlotFcns" , optimplotfunccount );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();

//
// Use all 3 plot functions
//
myfunctions = list ( optimplotfval , optimplotx , optimplotfunccount );
opt = optimset ( "PlotFcns" , myfunctions );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();


