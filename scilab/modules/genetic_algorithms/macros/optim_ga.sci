// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2014 - Michael Baudin <michael.baudin@contrib.scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_ga(ga_f, pop_size, nb_generation, p_mut, p_cross, Log, param)

    [nargout, nargin] = argn();

    if ~isdef("param", "local") then
        param = [];
    end

    [codage_func, err]    = get_param(param, "codage_func", coding_ga_identity);
    [init_func, err]      = get_param(param, "init_func", init_ga_default);
    [crossover_func, err] = get_param(param, "crossover_func", crossover_ga_default);
    [mutation_func, err]  = get_param(param, "mutation_func", mutation_ga_default);
    [selection_func, err] = get_param(param, "selection_func", selection_ga_elitist);
    [nb_couples, err]     = get_param(param, "nb_couples", 100);
    [pressure, err]       = get_param(param, "pressure", 0.05);
    [output_func, err] = get_param(param, "output_func", output_ga_default);

    if ~isdef("ga_f", "local") then
        error(sprintf(gettext("%s: ga_f is mandatory"), "optim_ga"));
    else
        if typeof(ga_f) == "list" then
            deff("y = _ga_f(x)", "y = ga_f(1)(x, ga_f(2:$))");
        else
            deff("y = _ga_f(x)", "y = ga_f(x)");
        end
    end

    if ~isdef("pop_size", "local") then
        pop_size = 100;
    end
    if ~isdef("nb_generation", "local") then
        nb_generation = 10;
    end
    if ~isdef("p_mut", "local") then
        p_mut = 0.1;
    end
    if ~isdef("p_cross", "local") then
        p_cross = 0.7;
    end
    if ~isdef("Log", "local") then
        Log = %F;
    end

    // Initialization of the population
    Pop = list();
    Pop = init_func(pop_size, param);

    if (nargout >= 3) then
        pop_init = Pop;
    end

    // Code the individuals
    Pop = codage_func(Pop, "code", param);

    for i = 1:length(Pop)
        FObj_Pop(i) = _ga_f(Pop(i));
    end

    if (nargout == 4) then
        fobj_pop_init = FObj_Pop;
    end

    FObj_Pop_Max = max(FObj_Pop);
    FObj_Pop_Min = min(FObj_Pop);

    // Normalization of the efficiency
    Efficiency = (1 - pressure) * (FObj_Pop_Max - FObj_Pop) / max([FObj_Pop_Max - FObj_Pop_Min %eps]) + pressure;

    // The genetic algorithm
    for i = 1:nb_generation
        //
        // Selection
        //
        Indiv1 = list();
        Indiv2 = list();
        Wheel = cumsum(Efficiency);
        for j = 1:nb_couples
            // Selection of the first individual in the couple
            Shoot = grand(1, 1, "unf", 0, Wheel($));
            Index = find(Shoot <= Wheel, 1);
            Indiv1(j)      = Pop(Index);
            FObj_Indiv1(j) = FObj_Pop(Index);
            // Selection of the second individual in the couple
            Shoot = grand(1, 1, "unf", 0, Wheel($));
            Index = 1;
            Index = find(Shoot <= Wheel, 1);
            Indiv2(j)      = Pop(Index);
            FObj_Indiv2(j) = FObj_Pop(Index);
        end
        //
        // Crossover
        //
        for j = 1:nb_couples
            if (p_cross>grand(1, 1, "def")) then
                [x1, x2] = crossover_func(Indiv1(j), Indiv2(j), param);
                Indiv1(j) = x1;
                Indiv2(j) = x2;
                ToCompute_I1(j) = %T;
                ToCompute_I2(j) = %T;
            else
                ToCompute_I1(j) = %F;
                ToCompute_I2(j) = %F;
            end
        end
        //
        // Mutation
        //
        for j = 1:nb_couples
            if (p_mut>grand(1, 1, "def")) then
                x1 = mutation_func(Indiv1(j), param);
                Indiv1(j) = x1;
                ToCompute_I1(j) = %T;
            end
            if (p_mut>grand(1, 1, "def")) then
                x2 = mutation_func(Indiv2(j), param);
                Indiv2(j) = x2;
                ToCompute_I2(j) = %T;
            end
        end
        //
        // Computation of the objective functions
        //
        for j = 1:nb_couples
            if ToCompute_I1(j) then FObj_Indiv1(j) = _ga_f(Indiv1(j)); end
            if ToCompute_I2(j) then FObj_Indiv2(j) = _ga_f(Indiv2(j)); end
        end

        // Reinit ToCompute lists
        ToCompute_I1 = ToCompute_I1 & %F;
        ToCompute_I2 = ToCompute_I2 & %F;

        //
        // Recombination
        //
        [Pop, FObj_Pop] = selection_func(Pop, Indiv1, Indiv2, FObj_Pop, FObj_Indiv1, FObj_Indiv2, [], [], [], param);

        // Callback for plotting / printing intermediate results or stopping the algorithm
        if (Log) then
            stop = output_func(i, nb_generation, Pop, FObj_Pop, param);
            if (stop) then
                break
            end
        end
    end

    pop_opt  = Pop;
    pop_opt  = codage_func(pop_opt, "decode", param);
    fobj_pop_opt = FObj_Pop;
endfunction
