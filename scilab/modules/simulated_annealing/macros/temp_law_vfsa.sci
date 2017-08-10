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

function T = temp_law_vfsa(T, step_mean, step_var, temp_stage, n, param)
    if (~isdef("param","local")) then
        param = []; // First create the empty param var
    end

    [c,err]         = get_param(param,"c",0.01);
    [dimension,err] = get_param(param,"dimension",3);

    T = T * (n/(n+1))^(1/dimension);
endfunction
