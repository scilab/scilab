// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
