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

function [Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_default(Indiv1,Indiv2,param)
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [Beta,err]      = get_param(param,"beta",0);
    [MinBounds,err] = get_param(param,"minbound",-2*ones(size(Indiv1,1),size(Indiv1,2)));
    [MaxBounds,err] = get_param(param,"maxbound",2*ones(size(Indiv1,1),size(Indiv1,2)));

    mix = (1 + 2*Beta)*grand(1,1,"def") - Beta;
    Crossed_Indiv1 =     mix*Indiv1 + (1-mix)*Indiv2;
    Crossed_Indiv2 = (1-mix)*Indiv1 +     mix*Indiv2;

    Crossed_Indiv1 = max(min(Crossed_Indiv1, MaxBounds),MinBounds);
    Crossed_Indiv2 = max(min(Crossed_Indiv2, MaxBounds),MinBounds);
endfunction
