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


function [slb,u]=balreal(a)

    [lhs,rhs]=argn(0)

    if typeof(a)<>"state-space" then error(91,1),end
    [a,b,c,d,x0,dom]=a(2:7);
    if dom==[] then error(96,1),end
    domain="c";
    if dom<>"c" then domain="d",end
    wc=lyap(a',-b*b',domain)
    wo=lyap(a,-c'*c,domain)
    r=chol(wo);x=r*wc*r';[u,s,v]=svd(x);s=diag(s);
    ns=size(s,"*");
    lli=sqrt(sqrt(s));ll=ones(ns,1)./lli
    ri=inv(r)*v;r=v'*r;
    a=r*a*ri;b=r*b;c=c*ri
    a=diag(ll)*a*diag(lli)
    b=diag(ll)*b
    c=c*diag(lli)
    slb=syslin(dom,a,b,c,d,diag(ll)*r*x0),
    if lhs==2 then u=ri*diag(lli),end
endfunction
