// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i = modulo(n, m)
    //i=modulo(n,m) returns  n modulo m.

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"modulo", 2));
    end

    if ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 1));
    end

    if ~isreal(m) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 2));
    end

    if size(n,'*')==1 then
        i = zeros(m);
        k = find(m==0);
        i(k) = n - int(n ./ m(k)) .* m(k);
        k = find(m~=0);
        i(k) = n-int(n./m(k)).*m(k);
    elseif size(m,'*')==1 then
        i = zeros(n);
        if m == 0 then
            i = n - int(n ./ m) .* m;
        else
            i = n-int(n./m).*m;
        end
    else
        if or(size(n) <> size(m)) then 
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"modulo"));
        end
        i = zeros(n);
        k = find(m==0);
        i(k) = n(k) - int(n(k) ./ m(k)) .* m(k);
        k = find(m~=0);
        i(k) = n(k) - int(n(k)./m(k)).*m(k);
    end
endfunction





