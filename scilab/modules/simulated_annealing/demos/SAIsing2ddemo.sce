//////////////////////////////////////////////////////
// example of use of the simulated annealing method //
//////////////////////////////////////////////////////

function demo_sa_2()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("SAIsing2ddemo.sce");
    my_handle.info_message = _("Please wait ...");

    lines(0);
    old_funcprot = funcprot();
    funcprot(0);

    // Loading the test problem

    path = get_absolute_file_path("SAIsing2ddemo.sce");

    getd(path + "/Ising");

    // Loading the neighborhood function for the ising problem

    getd(path + "/.");

    Proba_start = 0.8;
    It_intern = 1000;
    It_extern = 30;
    It_Pre  = 100;
    Log     = %T;
    alpha   = 0.9;

    Ising_Dim   = 10;
    Ising_Proba = 0.3;
    J     = 1.1;
    H     = 0.7;
    Magnet  = "%T";
    Connect = "%T";

    // For the minimization case, everything must be at -1 or 1 in the optimal solution
    deff("y=f(x)","y = ising2d(x,"+string(J)+","+string(H)+","+Magnet+","+Connect+")");
    // For the maximization case, we must have a checker board solution (+1 -1 +1 -1 ....)
    //deff("y=f(x)","y = - ising2d(x,"+string(J)+","+string(H)+","+Magnet+","+Connect+")");

    x0 = init_ising2d(Ising_Dim, Ising_Proba);

    /////////////////////////
    // Simulated Annealing //
    /////////////////////////

    printf("\nSA: geometrical decrease temperature law\n");

    sa_params = init_param();
    sa_params = add_param(sa_params,"dimension",10);
    sa_params = add_param(sa_params,"proba",0.05);
    sa_params = add_param(sa_params,"neigh_func", neigh_func_ising2d); // Required because this operator is specific to the ising2d problem
    sa_params = add_param(sa_params,"accept_func", accept_func_default); // Optional
    sa_params = add_param(sa_params,"temp_law", temp_law_default); // Optional
    sa_params = add_param(sa_params,"alpha",alpha); // For the temperature decreasing law

    T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);

    printf("Initial temperature T0 = %f\n", T0);

    [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log, sa_params);

    printf("optimal solution:\n"); disp(x_opt);
    printf("value of the objective function = %f\n", f_opt);

    if is_handle_valid(my_handle) then
        plot_ising2d(x_opt);
        my_handle.info_message = "";
    end

    funcprot(old_funcprot);

endfunction

demo_sa_2();
clear demo_sa_2;