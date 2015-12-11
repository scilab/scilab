// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
