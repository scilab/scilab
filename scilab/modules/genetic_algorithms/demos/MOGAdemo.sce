// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Yann COLLETTE
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.
////////////////////////////////////////
// Demo of the MOGA Genetic Algorithm //
////////////////////////////////////////

function demo_MOGA()

    /////////////////////////
    // Deb 1 test function //
    // Convex Pareto set   //
    /////////////////////////

    function Res = min_bd_deb_1(n)
        if ~isdef("n","local") then n = 10; end;
        Res = zeros(n, 1);
    endfunction

    function Res = max_bd_deb_1(n)
        if ~isdef("n","local") then n = 10; end;
        Res = ones(n, 1);
    endfunction

    function f = get_opti_deb_1(x)
        f1_x1 = x(1);
        g_x2  = 1;
        h   = 1 - sqrt(f1_x1 / g_x2);

        f(1,1) = f1_x1;
        f(1,2) = g_x2 * h;
    endfunction

    function f = deb_1(x)
        f1_x1 = x(1);
        g_x2  = 1 + 9 * sum((x(2:$)-x(1)).^2) / (length(x) - 1);
        h   = 1 - sqrt(f1_x1 / g_x2);

        f(1, 1) = f1_x1;
        f(1, 2) = g_x2 * h;
    endfunction

    funcname = "deb_1";

    // example of use of the genetic algorithm

    PopSize   = 100;
    Proba_cross = 0.5;
    Proba_mut   = 0.3;
    NbGen     = 4;
    NbCouples   = 110;
    Log     = %T;
    nb_disp   = 10; // Nb point to display from the optimal population
    pressure  = 0.1;

    ga_params = init_param();
    // Parameters to adapt to the shape of the optimization problem
    ga_params = add_param(ga_params, "minbound", eval("min_bd_" + funcname + "(2)"));
    ga_params = add_param(ga_params, "maxbound", eval("max_bd_" + funcname + "(2)"));
    ga_params = add_param(ga_params, "dimension", 2);
    ga_params = add_param(ga_params, "beta", 0);
    ga_params = add_param(ga_params, "delta", 0.1);
    // Parameters to fine tune the Genetic algorithm. All these parameters are optional for continuous optimization
    // If you need to adapt the GA to a special problem, you
    ga_params = add_param(ga_params, "init_func", init_ga_default);
    ga_params = add_param(ga_params, "crossover_func", crossover_ga_default);
    ga_params = add_param(ga_params, "mutation_func", mutation_ga_default);
    ga_params = add_param(ga_params, "codage_func", coding_ga_identity);
    ga_params = add_param(ga_params, "selection_func", selection_ga_elitist);
    //ga_params = add_param(ga_params, 'selection_func', selection_ga_random);
    ga_params = add_param(ga_params, "nb_couples", NbCouples);
    ga_params = add_param(ga_params, "pressure", pressure);

    deff("y=fobjs(x)","y = " + funcname + "(x);");

    ////////////////////
    // MOGA Algorithm //
    ////////////////////

    printf("\n");
    printf(gettext("%s: optimization starting, please wait ...\n"),"MOGA");

    [pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_moga(fobjs, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

    [f_pareto,pop_pareto] = pareto_filter(fobj_pop_opt, pop_opt);

    my_handle = scf(100001);
    clf(my_handle,"reset");

    if (size(fobj_pop_opt,2) == 2) then
        drawlater;
        subplot(2, 1, 1);
        printf("plotting init population ...\n");
        plot(fobj_pop_init(:,1),fobj_pop_init(:,2),"r.");

        if isdef("get_opti_" + funcname) then
            t = 0 : 0.01 : 1;
            for i = 1 : length(t)
                y_t(i,:) = eval("get_opti_" + funcname + "(t(" + string(i) + "))");
            end
            plot(y_t(:,1), y_t(:,2), "k-");
        end

        legend(["Init","Pareto front."]);
        xtitle("Objective function space","f1","f2");

        subplot(2,1,2);
        printf("plotting final population ...\n");
        plot(fobj_pop_opt(:,1),fobj_pop_opt(:,2),"g.");
        printf("plotting Pareto population ...\n");
        plot(f_pareto(:,1),f_pareto(:,2),"k.");

        if isdef("get_opti_"+funcname) then
            t = 0:0.01:1;
            for i=1:size(t,"*")
                y_t(i,:) = eval("get_opti_" + funcname + "(t(" + string(i) + "))");
            end
            plot(y_t(:,1), y_t(:,2), "k-");
        end

        legend(["Final pop.","Pareto pop.","Pareto front."]);
        xtitle("Objective function space","f1","f2");
        drawnow;
    end

    demo_viewCode("MOGAdemo.sce");

endfunction

demo_MOGA();
clear demo_MOGA;

