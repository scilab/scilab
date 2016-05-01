// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier: standardized and added tests
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- CLI SHELL MODE -->

// <-- ENGLISH IMPOSED -->

// Minimizing a quadratic function in dimension 1.
deff("y = test_func(x)", "y = x^2;");
ga_params = init_param();
ga_params = add_param(ga_params, "dimension", 1);
[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_ga(test_func, 50, 10, 0.1, 0.7, %F, ga_params);

assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));


// In the following example, we customize all the options in order to show all the features of the algorithm,
// and we increase the dimension.
function y = f(x)
    y = sum(x.^2);
endfunction
PopSize     = 100;
Proba_cross = 0.7;
Proba_mut   = 0.1;
NbGen       = 10;
NbCouples   = 110;
Log         = %T;
pressure    = 0.05;
ga_params = init_param();
// Parameters to adapt to the shape of the optimization problem
ga_params = add_param(ga_params, "minbound", [-2; -2]);
ga_params = add_param(ga_params, "maxbound", [2; 2]);
ga_params = add_param(ga_params, "dimension", 2);
ga_params = add_param(ga_params, "beta", 0);
ga_params = add_param(ga_params, "delta", 0.1);
// Parameters to fine tune the Genetic algorithm.
// All these parameters are optional for continuous optimization
// If you need to adapt the GA to a special problem, you
ga_params = add_param(ga_params, "init_func", init_ga_default);
ga_params = add_param(ga_params, "crossover_func", crossover_ga_default);
ga_params = add_param(ga_params, "mutation_func", mutation_ga_default);
ga_params = add_param(ga_params, "codage_func", coding_ga_identity);
ga_params = add_param(ga_params, "selection_func", selection_ga_elitist);
//ga_params = add_param(ga_params, "selection_func", selection_ga_random);
ga_params = add_param(ga_params, "nb_couples", NbCouples);
ga_params = add_param(ga_params, "pressure", pressure);

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_ga(f, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);
assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));


// Customizing the init function, which computes the initial population.
function Pop_init = myinitga(popsize, param)
    // This message is to be displayed in the console
    // for demonstration purpose only :
    // remove it in a real application!
    disp("Initializing the Population with grand")
    // We deal with some parameters to take into account
    // the boundary of the domain and the neighborhood size
    [Dim, err]       = get_param(param, "dimension", 2)
    [MinBounds, err] = get_param(param, "minbound", -2*ones(1, Dim))
    [MaxBounds, err] = get_param(param, "maxbound", 2*ones(1, Dim))
    // Pop_init must be a list()
    Pop_init = list()
    nr = size(MaxBounds, 1)
    nc = size(MaxBounds, 2)
    for i=1:popsize
        u = grand(nr, nc, "def")
        Pop_init(i) = (MaxBounds - MinBounds).*u + MinBounds
    end
endfunction
ga_params = init_param();
// Parameters to adapt to the shape of the optimization problem
ga_params = add_param(ga_params, "minbound", [-2; -2]);
ga_params = add_param(ga_params, "maxbound", [2; 2]);
ga_params = add_param(ga_params, "dimension", 2);
ga_params = add_param(ga_params, "init_func", myinitga);

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_ga(f, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);
assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));


// Passing a list to the optim_ga function, where the first element of the list is
// the function and the remaining elements are the extra parameters. Dimension 3.
function y = f(x, a1, a2)
    y = a1*sum(x.^2) + a2;
endfunction
ga_params = init_param();
// Parameters to control the initial population.
ga_params = add_param(ga_params, "dimension", 3);
// Pass the extra parameters to the objective function
a1 = 12;
a2 = 7;
myobjfun = list(f, a1, a2);

// Optimize !
[pop_opt, fobj_pop_opt] = optim_ga(myobjfun, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);
assert_checkequal(length(pop_opt), length(pop_init));
assert_checkequal(size(fobj_pop_opt), size(fobj_pop_init));
