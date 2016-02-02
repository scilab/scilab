// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [gc]=ctr_gram(a,b,domaine)
    //!

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "constant" then
        if rhs<2 then error(39); end;
        if rhs==2 then
            domaine="c";
        else
            if and(domaine<>["d","c"]) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "ctr_gram",3,"''d'', ''c''"));
            end
        end;
        [m,n]=size(a)
        if m<>n then error(20,1),end
        [mb,nb]=size(b);if mb<>n then error(60),end
    case "state-space" then
        if rhs>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"ctr_gram",1)),
        end
        [a,b,domaine]=a([2,3,7])
        if domaine==[] then
            warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"ctr_gram",1));
            domaine="c";
        elseif type(domaine)==1 then
            domaine="d",
        end
        [n,n]=size(a)
    case "rational" then
        if rhs>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"ctr_gram",1)),
        end
        a=tf2ss(a)
        [a,b,domaine]=a([2,3,7])
        if domaine==[] then
            warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"ctr_gram",1));
            domaine="c";
        elseif type(domaine)==1 then
            domaine="d",
        end
        [n,n]=size(a)
    else
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"ctr_gram",1))
        else
            error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"ctr_gram",1))
        end
    end
    s=spec(a)
    if (domaine=="c"&max(real(s))>=0)|(domaine=="d"&max(abs(s))>=1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"ctr_gram",1));
    end
    gc=lyap(a',-b*b',domaine)
endfunction
