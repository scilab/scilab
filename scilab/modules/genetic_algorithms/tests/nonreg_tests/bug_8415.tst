// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 8415 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8415
//
// <-- Short Description -->
// optim_moga(), optim_nsga() and optim_nsga2() can now take list as arguments, as announced in the doc.

// With a list for cost function
function f = deb_2(x, p1, p2)
    f1_x1 = x(1);
    g_x2  = p1 + p2 * sum((x(2:$)-x(1)).^2) / (length(x) - 1);
    h     = 1 - sqrt(f1_x1 / g_x2);
    f(1,1) = f1_x1;
    f(1,2) = g_x2 * h;
endfunction

PopSize     = 100;
Proba_cross = 0.5;
Proba_mut   = 0.3;
NbGen       = 4;
NbCouples   = 110;
Log         = %T;
nb_disp     = 10; // Nb point to display from the optimal population
pressure    = 0.1;

ga_params = init_param();
ga_params = add_param(ga_params, "dimension", 2);
ga_params = add_param(ga_params, "minbound", zeros(2, 1));
ga_params = add_param(ga_params, "maxbound", ones(2, 1));

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_moga(list(deb_2, 1, 9), PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_nsga(list(deb_2, 1, 9), PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_nsga2(list(deb_2, 1, 9), PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));
