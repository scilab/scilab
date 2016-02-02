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


function [n,m,xt,yt]=copfac(g,polf,polc,tol)
    //[n,m,xt,yt]=copfac(G,[polf,polc,[tol]]) returns a right coprime
    //factorization of g :
    // g = n*m^-1 where n and m are stable, proper and right coprime.
    // (ie. [n m] left-invertible with stability)
    // xt and yt satisfy:
    //    [xt -yt].[m n]' = eye (Bezout identity)
    // G is assumed stabilizable and detectable.
    //-- G is is linear system (syslin list)
    //-- polf are polc respectively the poles of xt and yt
    //   and the poles of n and m .
    //   These are optional arguments with defautl values -1.
    //-- tol is a threshold for detecting stable poles.
    //!

    [lhs,rhs]=argn(0),
    if rhs<1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"copfac",1))
    end
    select typeof(g)
    case "rational" then
        g=tf2ss(g)
    case "state-space" then
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"copfac",1))
    end
    if g.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"copfac",1))
    end

    [r,p,t]=size(g);
    [a,b,c,d]=abcd(g),
    [n1,u1]=contr(a,b),[n2,u2]=contr(a',c'),
    select rhs,
    case 1 then
        polc=-ones(1,n1),polf=-ones(1,n2),tol=1000*%eps,
    case 2 then
        itol=2
        tol=polf,polc=-ones(1,n1),polf=-ones(1,n2),
    case 3 then
        tol=1000*%eps,
    else
        itol=4
    end,
    if type(tol)<>1|size(tol,"*")<>1|~isreal(tol)|tol<=0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be a positive scalar.\n"),"copfac",itol))
    end
    if type(polf)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector of floats expected.\n"),"copfac",2))
    end
    if size(polf,"*")<>n2 then
        error(msprintf(gettext( "%s: Wrong size for argument #%d: %d expected.\n"),"copfac",2,n2))
    end
    if type(polc)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector of floats expected.\n"),"copfac",3))
    end
    if size(polc,"*")<>n1 then
        error(msprintf(gettext( "%s: Wrong size for argument #%d: %d expected.\n"),"copfac",3,n1))
    end


    //--------------------
    if n1<>t then
        w1=u1(:,n1+1:t),a1=w1'*a*w1,
        no=norm(a1),
        if max(real(spec(a1)))>no*tol then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Stabilizable system expected.\n"),"copfac",1)),
        end,
    end,
    //------------------
    if n2<>t then
        w2=u2(:,n2+1:t),a2=w2'*a*w2,
        no=norm(a2),
        if max(real(spec(a2)))>no*tol then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Detectable system expected.\n"),"copfac",1)),
        end,
    end,
    //-----------------------------
    v1=u1(:,1:n1),a1=v1'*a*v1,b1=v1'*b,
    k1=ppol(a1,b1,polc)*v1',
    ak1=a-b*k1,ck1=c-d*k1,
    v2=u2(:,1:n2),a2=v2'*a*v2,c2=c*v2,
    k2=ppol(a2',c2',polf),
    k2=v2*k2',
    ak2=a-k2*c,bk2=b-k2*d,
    //-------------------
    m=syslin("c",ak1,b,-k1,eye(p,p)),
    n=syslin("c",ak1,b,ck1,d),
    xt=syslin("c",ak2,-bk2,-k1,eye(p,p)),
    yt=syslin("c",ak2,k2,-k1),
endfunction
