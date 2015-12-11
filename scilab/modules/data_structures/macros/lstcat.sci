// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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
