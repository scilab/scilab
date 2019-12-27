//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF --> 
//
// <-- Non-regression test for bug 8964 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8964
//
// <-- Short Description -->
// The optim_* function may not create pop_init.
// 1. Define the Rastrigin function.
function y = rastriginV ( x1 , x2  )
  // Vectorized function for contouring.
  y = x1.^2 + x2.^2-cos(12*x1)-cos(18*x2)
endfunction

function y = rastrigin ( x )
  // Non-vectorized function for optimization.
  y = rastriginV ( x(1) , x(2) )
endfunction

// 2. Compute the optimum.
PopSize     = 100;
Proba_cross = 0.7;
Proba_mut   = 0.1;
NbGen       = 10;
NbCouples   = 110;
Log         = %T;

ga_params = init_param();
ga_params = add_param(ga_params,"minbound",[-1 -1]');
ga_params = add_param(ga_params,"maxbound",[1 1]');
ga_params = add_param(ga_params,"dimension",2);


[pop_opt, fobj_pop_opt, pop_init] = optim_ga(rastrigin, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);
// If it fails like in the past, the following tests will fail

assert_checkequal(size(pop_opt),100);
assert_checkequal(size(pop_init),100);
