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

function x_neigh = neigh_func_csa(x_current, T, param)
    if ~isdef("param","local") then
        param = [];
    end

    [Sigma,err] = get_param(param,"sigma",ones(size(x_current,1), size(x_current,2)));
    [Min,err]   = get_param(param,"min_bound",-%inf*ones(size(x_current,1),size(x_current,2)));
    [Max,err]   = get_param(param,"max_bound",%inf*ones(size(x_current,1),size(x_current,2)));

    x_neigh = x_current + Sigma.*sqrt(2)*T*grand(1,1,"nor",0,1);
    x_neigh = max(min(x_neigh, Max),Min);
endfunction
