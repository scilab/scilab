// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function l=lstcat(varargin)
    // concat inputs in a list

    rhs=argn(2);
    l = list();

    if rhs==0 then
        return;
    end

    for i=1:rhs
        var = varargin(i);
        if type(var) == 15 then
            for j = 1 : size(var)
                l($ + 1) = var(j);
            end
        elseif type(var) <= 10 then
            l($ + 1) = var;
        end
    end
endfunction
