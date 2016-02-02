// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
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
// <-- ENGLISH IMPOSED -->

// Test that we can run with default values for parameters
function y=test_func(x)
    y=x^2
endfunction

rand("seed",0);
x0 = 10;
ItExt = 30;
ItInt = 50;
Log = %f;
ItMX = 100;
T_init = compute_initial_temp(x0, test_func, 0.8, ItMX);
[x_best, f_best, mean_list, var_list, temp_list, f_history, x_history,iter] = optim_sa(x0, test_func, ItExt, ItInt, T_init, Log);

assert_checkalmostequal ( x_best , 0.0 , [] , 1.e-1 );
assert_checkalmostequal ( f_best , 0.0 , [] , 1.e-1 );
assert_checkequal ( size(mean_list) , [1 ItExt] );
assert_checkequal ( size(var_list) , [1 ItExt] );
assert_checkequal ( size(temp_list) , [1 ItExt] );
assert_checkequal ( size(f_history) , ItExt );
assert_checkequal ( size(x_history) , ItExt );
assert_checkequal ( iter>0 , %t );

///////////////////////////////////////////
// Test that we can configure our own neighbour function
function f = quad ( x )
    p = [4 3];
    f = (x(1) - p(1))^2 + (x(2) - p(2))^2
endfunction

// We produce a neighbor by adding some noise to each component of a given vector
function x_neigh = myneigh_func ( x_current, T , param)
    nxrow = size(x_current,"r")
    nxcol = size(x_current,"c")
    sa_min_delta = -0.1*ones(nxrow,nxcol);
    sa_max_delta = 0.1*ones(nxrow,nxcol);
    x_neigh = x_current + (sa_max_delta - sa_min_delta).*rand(nxrow,nxcol) + sa_min_delta;
endfunction

rand("seed",0);
x0          = [2 2];
Proba_start = 0.7;
It_Pre      = 100;
It_extern   = 50;
It_intern   = 50;

saparams = init_param();
saparams = add_param(saparams,"neigh_func", myneigh_func);

T0 = compute_initial_temp(x0, quad, Proba_start, It_Pre, saparams);
Log = %f;
[x_opt, f_opt] = optim_sa(x0, quad, It_extern, It_intern, T0, Log,saparams);
assert_checkalmostequal ( x_opt , [4 3] ,  1.e-1 );
assert_checkalmostequal ( f_opt , 0 ,  [] , 1.e-1 );

///////////////////////////////////////////
// Test that an additional parameter can be passed to the cost function

function f = quadp ( x , p )
    f = (x(1) - p(1))^2 + (x(2) - p(2))^2
endfunction

rand("seed",0);
x0 = [-1 -1];
p = [4 3];
T0 = compute_initial_temp(x0, list(quadp,p) , Proba_start, It_Pre);
[x_opt, f_opt] = optim_sa(x0, list(quadp,p) , 30, 30, T0, %f);
assert_checkalmostequal ( x_opt , [4 3] ,  1.e-1 );
assert_checkalmostequal ( f_opt , 0 ,  [] , 1.e-1 );

///////////////////////////////////////////
// Test with a plot function, which serves also as a stop function.

function f = quad ( x )
    p = [4 3];
    f = (x(1) - p(1))^2 + (x(2) - p(2))^2
endfunction

// See that the stop variable becomes true when the function value is near zero.
// The threshold is rather loose.
function stop = outfunc ( itExt , x_best , f_best , T , saparams )
    [mythreshold,err] = get_param(saparams,"mythreshold",0);
    v = format()
    format("e",10)
    sxbest = string(x_best)
    mprintf ( "Iter = #%-4d, \t x_best=[%12s %12s], f_best = %12s, T = %12s\n", itExt , sxbest(1), sxbest(2) , string(f_best) , string(T) )
    if ( v(1) == 0 ) then
        format("e",v(2))
    else
        format("v",v(2))
    end
    stop = ( abs(f_best) < mythreshold )
endfunction

rand("seed",0);
x0 = [-1 -1];
saparams = init_param();
saparams = add_param(saparams,"output_func", outfunc );
saparams = add_param(saparams,"mythreshold", 1.e-1 );

T0 = compute_initial_temp(x0, quad , 0.7, 100, saparams);
// Notice that the number of external iterations is %inf, so
// that the external loop never stops.
// This allows to check that the output function really allows to
// stop the algorithm.
[x_best, f_best, mean_list, var_list, temp_list, f_history, x_history , iter ] = optim_sa(x0, quad , 1e6, 100, T0, %f, saparams);
assert_checkalmostequal ( x_best , [4 3] ,  1.e-1 );
assert_checkalmostequal ( f_best , 0 ,  [] , 1.e-1 );
assert_checkequal ( iter > 0 , %t );


