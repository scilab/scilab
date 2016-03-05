// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function T_init = compute_initial_temp(x0, cit_f, proba_init, ItMX, param)

    if (~isdef("param","local")) then
        param = [];
    end

    [neigh_func,err]  = get_param(param,"neigh_func",neigh_func_default);
    [type_accept,err] = get_param(param,"type_accept","sa");

    if ~isdef("cit_f","local") then
        error(sprintf(gettext("%s: cit_f is mandatory"),"compute_initial_temp"));
    else
        if typeof(cit_f)=="list" then
            deff("y=_cit_f(x)","y=cit_f(1)(x, cit_f(2:$))");
        else
            deff("y=_cit_f(x)","y=cit_f(x)");
        end
    end

    f_list    = [];
    x_current = x0;
    f_current = _cit_f(x_current);
    f_list    = [f_list f_current];

    for i=1:ItMX
        x_current = neigh_func(x_current, 0, param);
        f_current = _cit_f(x_current);
        f_list = [f_list f_current];
    end

    NbInc = 0;
    f_sum = 0;

    for i=2:size(f_list,2)
        if (f_list(i-1)<f_list(i)) then
            NbInc = NbInc + 1;
            f_sum = f_sum + (f_list(i)-f_list(i-1));
        end
    end

    if (NbInc>0) then
        f_sum = f_sum / NbInc;
    end

    if type_accept=="sa" then
        // proba_init = exp(-delta_f/T_init) -> -delta_f / log(proba_init) = T_init
        T_init = - f_sum ./ log(proba_init);
    elseif type_accept=="vfsa" then
        T_init = abs(f_sum / log(1/proba_init - 1));
    else
        error(sprintf(gettext("%s: error - wrong accept type"),"compute_initial_temp"));
    end
endfunction
