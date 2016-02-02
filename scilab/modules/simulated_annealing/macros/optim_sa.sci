// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x_best, f_best, mean_list, var_list, temp_list, f_history, x_history , iter ] = optim_sa(x0, sa_f, ItExt, ItInt, T0, Log, param)
    // Simulated annealing
    // x0         : initial solution
    // f          : objective function
    // ItExt      : number of temperature decrease
    // ItInt      : number of iterations during one temperature step
    // T0         : initial temperature
    // Log        : print some message during the run of the optimization
    // param      : a parameter list. this list contains the neighobrhood ('neigh_func') and some parameters related to this neighborhood functions (see the
    //              related function to list the available parameters)

    [nargout, nargin] = argn();

    if ~isdef("param","local") then
        param = [];
    end

    [temp_law,err]    = get_param(param,"temp_law",temp_law_default);
    [neigh_func,err]  = get_param(param,"neigh_func",neigh_func_default);
    [accept_func,err] = get_param(param,"accept_func",accept_func_default);
    [output_func,err] = get_param(param,"output_func",[]);

    if (~isdef("Log","local")) then
        Log = %F;
    end

    if (nargout>=6) then
        f_history_defined = %T;
        f_history = list();
    else
        f_history_defined = %F;
    end

    if (nargout>=5) then
        temp_list_defined = %T;
        temp_list = [];
    else
        temp_list_defined = %F;
    end

    if (nargout>=7) then
        x_history_defined = %T;
        x_history = list();
    else
        x_history_defined = %F;
    end

    if ~isdef("sa_f","local") then
        error(gettext("optim_sa: sa_f is mandatory"));
    else
        if typeof(sa_f)=="list" then
            deff("y=_sa_f(x)","y=sa_f(1)(x, sa_f(2:$))");
        else
            deff("y=_sa_f(x)","y=sa_f(x)");
        end
    end

    T = T0;

    // Some variables needed to record the behavior of the SA
    var_list  = [];
    mean_list = [];
    temp_list = [];

    x_current = x0;
    f_current = _sa_f(x_current);

    x_best = x_current;
    f_best = f_current;

    for iter=1:ItExt
        if ( output_func <> [] ) then
            stop = output_func ( iter , x_best , f_best , T , saparams );
            if (stop) then
                break
            end
        end

        f_list = [];
        x_list = list();
        for j=1:ItInt
            x_neigh = neigh_func(x_current,T,param);
            f_neigh = _sa_f(x_neigh);
            if ((f_neigh<=f_current)|(accept_func(f_current,f_neigh,T)>grand(1,1,"def"))) then
                x_current = x_neigh;
                f_current = f_neigh;
            end

            f_list = [f_list f_current];

            if (f_best>f_current) then
                x_best = x_current;
                f_best = f_current;
            end

            if (x_history_defined) then
                x_list($+1) = x_current;
            end
        end

        if (temp_list_defined) then
            temp_list = [temp_list T];
        end
        if (x_history_defined) then
            x_history($+1) = x_list;
        end
        if (f_history_defined) then
            f_history($+1) = f_list;
        end

        // Computation of step_mean and step_var
        step_mean = mean(f_list);
        step_var  = stdev(f_list);
        mean_list = [mean_list step_mean];
        var_list  = [var_list step_var];

        if (Log) then
            printf(gettext("%s: Temperature step %d / %d - T = %f, E(f(T)) = %f var(f(T)) = %f f_best = %f\n"), "optim_sa", iter, ItExt, T, step_mean, step_var, f_best);
        end

        T = temp_law(T, step_mean, step_var, iter, max(size(x_current)), param);
    end
endfunction


