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

function slc=contrss(a,tol)

    [lhs,rhs]=argn(0)
    //
    if typeof(a)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: State-space form expected.\n"),"contrss",1));
    end

    if rhs==1 then tol=sqrt(%eps);end
    [a,b,c,d,x0,dom]=a(2:7)
    //
    [nc,u]=contr(a,b,tol*norm([a,b],1))
    u=u(:,1:nc)
    a=u'*a*u;b=u'*b;c=c*u
    slc=syslin(dom,a,b,c,d,u'*x0)
endfunction
