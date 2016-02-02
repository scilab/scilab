// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [y] = x_matrix(mes, x)

    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "x_matrix", 2));
    end

    if type(x)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "x_matrix", 2));
    end
    [n1,n2] = size(x);
    z = string(x);
    y = [];
    for i=1:n1,
        y = [y; strcat(z(i,:), ", ")];
    end
    y(1) = "["+y(1);
    for i=2:n1,
        y(i) = " "+y(i);
    end
    y(n1) = y(n1)+"]";
    y = evstr(x_dialog(mes,y));
endfunction
