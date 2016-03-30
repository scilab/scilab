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

function Slmin=minss(Sl,tol)

    [lhs,rhs]=argn(0)

    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: State-space form expected.\n"),"minss",1));
    end

    [a,b,c,d,x0,dom]=Sl(2:7)
    //observability
    if rhs<2 then tol=1.d-10*norm([a;c],1),end
    [nc,u1]=contr(a',c',tol)
    u=u1(:,1:nc)
    c=c*u;a=u'*a*u;b=u'*b,x0=u'*x0;

    //controllability
    if rhs<2 then tol=1.d-10*norm([a,b],1),end
    [no,u2]=contr(a,b,tol)
    u=u2(:,1:no)
    a=u'*a*u;b=u'*b;c=c*u

    //form the result
    Slmin=syslin(dom,a,b,c,d,u'*x0)
    //Would be nice to return U=U1*U2
endfunction
