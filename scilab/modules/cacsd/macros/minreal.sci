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

function [a,b,c]=minreal(a,b,c,domaine,tol)
    //

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "state-space" then
        if lhs<>1 then
            error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"),"minreal",1)),
        end;
        select rhs
        case 1 then
            istol = %f;
        case 2 then
            istol = %t,
            tol = b,
        else
            error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"minreal",1,2)),
        end;
        [a,b,c,d,x0,dom] = a(2:7);
        if dom == [] then
            error(96,1);
        end
        domaine="c";
        if dom<>"c" then
            domaine="d";
        end
    case "constant" then
        if lhs<>3 then
            error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"),"minreal",3)),
        end;
        select rhs
        case 4 then istol = %f
        case 5 then istol = %t,
        else
            error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"minreal",4,5));
        end;
    else
        error(91,1);
    end;
    //
    wc = lyap(a', -b*b', domaine);
    wo = lyap(a, -c'*c, domaine);
    if ~istol then
        [r,n] = equil1(wc,wo);
    else
        [r,n] = equil1(wc,wo,tol);
    end;
    n1 = n(1);
    ri = inv(r);
    r = r(1:n1,:);
    ri = ri(:,1:n1);
    a = r*a*ri;
    b = r*b;
    c = c*ri;
    if lhs == 1 then
        a = syslin(dom,a,b,c,d,r*x0);
    end
endfunction
