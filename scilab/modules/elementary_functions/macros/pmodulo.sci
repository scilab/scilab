// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - Samuel GOUGEON
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i = pmodulo(n, m)
    //i=pmodulo(n,m) the "positive modulo" of m et n.
    //i=n-floor(n./m).*m

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "pmodulo", 2));
    end

    if ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "pmodulo", 1));
    end

    if ~isreal(m) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "pmodulo", 2));
    end

    m = abs(m)  // else returns i<0 for m<0 : http://bugzilla.scilab.org/12373
    if size(n,'*') == 1 then
        i = zeros(m);
        k = find(m == 0)
        i(k) = n - floor(n ./ m(k)) .* m(k);
        k = find(m~=0);
        i(k) = n-floor(n./m(k)).*m(k);
    elseif size(m,'*') == 1 then
        i = zeros(n);
        if m == 0 then
            i = n - floor(n ./ m) .* m;
        else
            i = n-floor(n./m).*m;
        end
    else
        if or(size(n)<>size(m)) then 
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"pmodulo"));
        end
        i = zeros(n);
        k = find(m==0);
        i(k) = n(k) - floor(n(k) ./ m(k)) .* m(k);
        k = find(m~=0);
        i(k) = n(k)-floor(n(k)./m(k)).*m(k);
    end
endfunction
