// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x,k]=unique(x,orient)
    // extract unique components of a vector
    if argn(2)<2 then orient="*",end
    if size(x,orient)==1 then k = 1;return,end
    if orient=="*" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"g","d");
            keq = find( x(2:$) == x(1:$-1) );
            if keq<>[] then keq = keq+1;end
            x(keq) = [];
            k(keq) = [];
            k = k($:-1:1);
            x = x($:-1:1);
        else
            x = gsort(x,"g","d");
            x = x($:-1:1);
            x( find(x(2:$) == x(1:$-1)) ) = [];
        end
    elseif  orient==1|orient=="r" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"lr","d");
            keq = find(and(x(2:$,:) == x(1:$-1,:),"c"))
            if keq<>[] then keq = keq+1;end
            x(keq,:) = [];
            k(keq,:) = [];
            k = k($:-1:1,:);
            x = x($:-1:1,:);
        else
            x = gsort(x,"lr","i");
            x( find(and(x(2:$,:) == x(1:$-1,:),"c")),:) = [];
        end
    elseif  orient==2|orient=="c" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"lc","d");
            keq = find(and(x(:,2:$) == x(:,1:$-1),"r"))
            if keq<>[] then keq = keq+1;end
            x(:,keq) = [];
            k(:,keq) = [];
            k = k(:,$:-1:1);
            x = x(:,$:-1:1);
        else
            x = gsort(x,"lc","i");
            x(:, find(and(x(:,2:$) == x(:,1:$-1),"r")) ) = [];
        end
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %d,''%s'',%d or ''%s'' expected\n"),"unique",2,1,"r",2,"c"));
    end
endfunction
