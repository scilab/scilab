// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6774 -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6774
//
// <-- Short Description -->
//  Detailed description of the problem
// The use of get_param requires to
// be extra-careful when we get the value of an option which is a function.


function y = rastrigin(x)
  y = x(1)^2+x(2)^2-cos(12*x(1))-cos(18*x(2));
endfunction

function x_neigh = myneighfun (x_current, T,param)
  sa_min_delta = -0.1*ones(size(x_current,1),size(x_current,2));
  sa_max_delta = 0.1*ones(size(x_current,1),size(x_current,2));
  x_neigh = x_current + (sa_max_delta - sa_min_delta).*rand(size(x_current,1),size(x_current,2)) + sa_min_delta;
endfunction

function T_init = myclientfunction ( x0, cit_f, proba_init, ItMX, param)
  [neigh_func,err]  = get_param(param,"neigh_func",neigh_func_default);
  [type_accept,err] = get_param(param,"type_accept","sa");
  T_init = 0
endfunction

x0          = [2 2];
Proba_start = 0.7;
It_Pre      = 100;
t_extern   = 100;
t_intern   = 1000;

params = init_param();
params = add_param(params,"neigh_func", myneighfun);
T0 = myclientfunction ( x0, rastrigin, Proba_start, It_Pre, params);

// The test is : no warning must be generated.

