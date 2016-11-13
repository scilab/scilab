// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x,dim]=spaninter(a,b,tol)
    //[X,dim]=spaninter(a,b [,tol])  computes intersection of Range(A)
    // and Range(B)
    //
    // x(:,1:dim) is an orthogonal basis for A inter B.
    //            In the X basis A and B are respectively:
    //            X'*A and X'*B.
    // dim        dimension of subspace A inter B.
    // tol        threshold (sqrt(%eps) is the default value).

    [lhs,rhs]=argn(0);
    [na,ma]=size(a);[nb,mb]=size(b);
    if ma*na==0 then dim=0;x=eye(nb,nb);return;end
    if mb*nb==0 then dim=0;x=eye(na,na);return;end
    if rhs==2 then tol=sqrt(%eps);end
    if na <> nb then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"spaninter",1,2));
    end
    if mb > ma then [x,dim]=spaninter(b,a,tol),return,end
    [xp,ra]=rowcomp(a);x=xp'
    //test  trivial cases :
    //a surjective a inter b is b
    if ra == na then [xp,dim]=rowcomp(b),x=xp',return,end
    //a is zero
    if ra == 0 then dim=0,return,end
    b=xp*b;      //update

    // b2=vectors in b whch are in a
    up=1:ra;low=ra+1:na;
    [v1,k2]=colcomp(b(low,:));
    b1=b*v1;     //update
    bup=b1(up,:);blow=b1(low,:)
    if norm(blow,1) <= tol*norm(bup,1) then k2=0,end
    k1=mb-k2;
    if k1==0 then dim=0;return;end
    b2=b1(:,1:k1);  //intersection in base x
    if norm(b2,1) < tol*norm(b,1)*mb*nb then dim=0,return,end
    [u2p,dim]=rowcomp(b2(up,:));
    x(:,up)=x(:,up)*u2p'         //update
endfunction



