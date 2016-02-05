// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug  -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13421
//
// <-- Short Description -->
// Callback functions for genetic algorithms were missing
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Objective function
function y=f(x)
    y = sum(x.^2)
endfunction

// Output function with a stop criterion
function stop=output_ga_custom(gen_index, nb_generation, Pop, FObj_Pop, param)
    [threshold, err] = get_param(param, "threshold", 1E-10); // default value for the threshold
    printf(_("%s: iteration %d / %d \n"), "optim_ga", gen_index, nb_generation);
    printf(_("    min / max value found = %.4E / %.4E\n"), min(FObj_Pop), max(FObj_Pop));
    stop = %f
    if abs(max(FObj_Pop) - min(FObj_Pop)) < threshold then
        printf(_("    Stop criterion reached: Delta Max to Min under threshold"));
        stop = %t
    end
endfunction

PopSize     = 100;
Proba_cross = 0.7;
Proba_mut   = 0.1;
NbGen       = 20;
NbCouples   = 110;
Log         = %T;
pressure    = 0.05;

ga_params = init_param();
// Parameters to adapt to the shape of the optimization problem
ga_params = add_param(ga_params,"minbound",[-2; -2]);
ga_params = add_param(ga_params,"maxbound",[2; 2]);
ga_params = add_param(ga_params,"dimension",2);
ga_params = add_param(ga_params,"beta",0);
ga_params = add_param(ga_params,"delta",0.1);
// Parameters to fine tune the Genetic algorithm.
// All these parameters are optional for continuous optimization
// If you need to adapt the GA to a special problem, you
ga_params = add_param(ga_params,"init_func",init_ga_default);
ga_params = add_param(ga_params,"crossover_func",crossover_ga_default);
ga_params = add_param(ga_params,"mutation_func",mutation_ga_default);
ga_params = add_param(ga_params,"codage_func",coding_ga_identity);
ga_params = add_param(ga_params,"selection_func",selection_ga_elitist);

//ga_params = add_param(ga_params,"selection_func",selection_ga_random);
ga_params = add_param(ga_params,"nb_couples",NbCouples);
ga_params = add_param(ga_params,"pressure",pressure);

// Customized output function with a stop criterion added
ga_params = add_param(ga_params, "threshold", 1E-6); // User defined parameter for the output function
ga_params = add_param(ga_params, "output_func", output_ga_custom);

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = ..
optim_ga(f, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

assert_checktrue(max(fobj_pop_opt) - min(fobj_pop_opt) <= 1E-6);
