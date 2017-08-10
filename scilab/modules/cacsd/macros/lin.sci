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

function [a,b,c,d]=lin(sim,x0,u0)
    //Syntaxes : sl=lin(sim,x0,u0)    or
    //           [a,b,c,d]=lin(sim,x0,u0)
    //
    //linearization of the non-linear differential system [y,xdot]=sim(x,u)
    //around x0 , u0. (sim is a scilab macro which computes y and xdot).
    //output:
    //- linear system sl (syslin list)
    //- (a,b,c,d)
    //
    //  Example : Let ftz be the function passed to ode e.g.
    //            [zd]=ftz(t,z,u), and let us assume y=x.
    //            [z]=ode(x0,t0,tf,list(ftz,u) compute x(tf)
    //            Let simula be the following function:
    //            function [y,xd]=simula(x,u)
    //            xd=ftz(tf,x,u); y=x;
    //
    //            The tangent linear system sl is obtained by:
    //               [a,b,c,d]=lin(simula,z,u)
    //                sl = syslin('c',a,b,c,d,x0)
    //!

    [lhs,rhs]=argn(0)
    [n,w]=size(x0);[m,w]=size(u0);mpn=m+n
    nrm=norm([x0;u0]);if nrm<1 then nrm=1,end;
    [zz,nu]=colcomp(rand(mpn,mpn));
    d=10*%eps*nrm*zz(:,1:nu); [y,xd]=sim(x0,u0); y0=[xd;y];
    ab=[];for v=d,[y,xd]=sim(x0+v(1:n),u0+v(n+1:mpn)),
    ab=[ab,([xd;y]-y0)],end
    [p,w]=size(y);
    ab=ab/d;a=ab(1:n,1:n);b=ab(1:n,n+1:mpn)
    c=ab(n+1:n+p,1:n);d=ab(n+1:n+p,n+1:mpn)
    if lhs==1 then a=syslin("c",a,b,c,d,x0),end
endfunction
