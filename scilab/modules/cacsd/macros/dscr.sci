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


function [f,r]=dscr(a,dt,m)

    [lhs,rhs]=argn(0);
    if type(dt)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"dscr",2))
    end
    if size(dt,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"dscr",2))
    end
    if ~isreal(dt) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"),"dscr",2))
    end

    if dt<=0 then
        error(msprintf(gettext("%s: Input argument #%d must be strictly positive.\n"),"dscr",2))
    end

    select typeof(a)
    case "rational" then
        a=tf2ss(a);
        [a,b,c,d,x0,dom]=a(2:7);
    case "state-space" then
        [a,b,c,d,x0,dom]=a(2:7)
    else
        error(97,1),
    end;
    if dom<>"c" then
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"dscr",1))
    end
    [n1,m1]=size(b),
    s=expm([a,b;0*ones(m1,n1+m1)]*dt),
    f=s(1:n1,1:n1);g=s(1:n1,n1+1:n1+m1);
    if rhs==3 then
        s=expm([-a,m;0*a a']*dt),
        r=f*s(1:n1,n1+1:n1+n1),
    end;
    f=syslin(dt,f,g,c,d,x0)
endfunction
