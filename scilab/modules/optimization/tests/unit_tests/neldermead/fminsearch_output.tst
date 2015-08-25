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

// <-- ENGLISH IMPOSED -->

function y = rosenbrock ( x )
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
function stop = outfun ( x , optimValues , state )
    plot( x(1),x(2),".");
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
    mprintf ( "%d %s %d -%s- %s\n" , fc , string(fv) , it , pr , state )
    stop = %f
endfunction
opt = optimset ( "OutputFcn" , outfun);
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());
//
// Use several output functions
//
function stop = outfun2 ( x , optimValues , state )
    global __fig1__
    scf ( __fig1__ );
    plot( x(1),x(2),".");
    stop = %f
endfunction
function stop = outfun3 ( x , optimValues , state )
    global __fig2__
    scf ( __fig2__ );
    plot( x(1),x(2),"o");
    stop = %f
endfunction
myfunctions = list ( outfun2 , outfun3 );
global __fig1__
global __fig2__
__fig1__ = scf();
__fig2__ = scf();
opt = optimset ( "OutputFcn" , myfunctions );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(__fig1__);
close(__fig2__);
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
    plot(x(1),x(2),".");
endfunction
opt = optimset ( "PlotFcns" , plotfun);
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());
//
// Use several plot functions
//
function plotfun2 ( x , optimValues , state )
    global __fig1__
    scf ( __fig1__ );
    plot( x(1),x(2),".");
endfunction
function plotfun3 ( x , optimValues , state )
    global __fig2__
    scf ( __fig2__ );
    plot( x(1),x(2),"o");
endfunction
myfunctions = list ( plotfun2 , plotfun3 );
global __fig1__
global __fig2__
__fig1__ = scf();
__fig2__ = scf();
opt = optimset ( "PlotFcns" , myfunctions );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(__fig1__);
close(__fig2__);
//
// Use optimplotfval plot function
//
opt = optimset ( "PlotFcns" , optimplotfval );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());
//
// Use optimplotx plot function
//
opt = optimset ( "PlotFcns" , optimplotx );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());
//
// Use optimplotfunccount plot function
//
opt = optimset ( "PlotFcns" , optimplotfunccount );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());

//
// Use all 3 plot functions
//
myfunctions = list ( optimplotfval , optimplotx , optimplotfunccount );
opt = optimset ( "PlotFcns" , myfunctions );
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(gcf());
close(gcf());
close(gcf());

//
// Use output function to stop the algorithm.
// This sets the exitflag to -1.
function stop = outfunStop ( x , optimValues , state )
    fv = optimValues.fval;
    stop = ( fv < 1.e-5 )
endfunction
opt = optimset ( "OutputFcn" , outfunStop);
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-2 );
assert_checktrue ( fval < 1e-5 );
assert_checkequal ( exitflag , -1 );



