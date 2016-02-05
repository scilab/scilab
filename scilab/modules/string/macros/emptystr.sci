// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function result = emptystr(varargin)
    [lhs, rhs] = argn(0);

    if rhs == 1 then
        //emptystr(x)
        s = size(varargin(1));
        p = prod(s);
        if p == 0 then
            //emptystr([])
            result = [];
        else
            result(1:p) = "";
            result = matrix(result, s);
        end
    elseif rhs == 2 then
        //emptystr(x,y)
        x = varargin(1);
        y = varargin(2);

        if type(x) <> 1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), "emptystr", 1));
        end
        if type(y) <> 1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), "emptystr", 2));
        end

        if x == 0 | y == 0 then
            result = [];
        else
            result(x,y) = "";
        end
    else
        //emptystr() or emptystr(x,y, ... )
        result = "";
    end
endfunction

