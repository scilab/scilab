// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
