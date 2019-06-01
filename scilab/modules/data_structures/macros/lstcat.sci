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


function l = lstcat(varargin)
    // concat inputs in a list

    rhs = argn(2);
    l = list();
    if rhs==0 then
        return;
    end

    l = varargin(1)
    if type(l)<>15 then
        l = list(l)
        L = 1
    else
        L = length(l)
    end

    for i = 2:rhs
        var = varargin(i);
        if type(var) == 15 then
            Lvar = length(var)
            if Lvar>0
                if Lvar < 100
                    l(L+Lvar) = var($)
                    for j = 1:Lvar-1
                        l(L+j) = var(j)
                    end
                else
                    l = list(l(:), var(:))
                end
                L = L + Lvar
            end
        else
            l($ + 1) = var;
            L = L + 1;
        end
    end
endfunction
