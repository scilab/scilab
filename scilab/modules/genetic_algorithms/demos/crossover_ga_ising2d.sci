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

function [Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_ising2d(Indiv1,Indiv2,param)
    if ~isdef("param","local") then
        param = [];
    end

    Index_x_min = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
    Index_x_max = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
    Index_y_min = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
    Index_y_max = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);

    if Index_x_min>Index_x_max then
        tmp         = Index_x_min;
        Index_x_min = Index_x_max;
        Index_x_max = tmp;
    end

    if Index_y_min>Index_y_max then
        tmp         = Index_y_min;
        Index_y_min = Index_y_max;
        Index_y_max = tmp;
    end


    Crossed_Indiv1 = Indiv1;
    Crossed_Indiv1(Index_x_min:Index_x_max,Index_y_min:Index_y_max) = Indiv2(Index_x_min:Index_x_max,Index_y_min:Index_y_max);
    Crossed_Indiv2 = Indiv2;
    Crossed_Indiv2(Index_x_min:Index_x_max,Index_y_min:Index_y_max) = Indiv1(Index_x_min:Index_x_max,Index_y_min:Index_y_max);
endfunction
