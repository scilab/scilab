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

function S=ss2des(Sl,flag)
    // Returns S=list('des',A,B,C,D,E) for Sl a state-space
    // system with Ds polynomial.
    // if flag=="withD" a maximal rank D matrix is returned in S
    // otherwise D=0;
    // Copyright INRIA
    [LHS,RHS]=argn(0);
    if RHS==1 then flag=[];end
    if RHS==2&flag<>"withD" then warning(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"ss2des",2,"withD"));end
    Ds=Sl(5);
    if type(Ds)==1 then
        if norm(Ds,1)==0 then S=Sl;return;end
        if norm(Ds,1)<>0 then Sl(5)=Ds+poly(0,"s")*0*Ds;end
    end
    [A2,B2,C2,Ds]=Sl(2:5);
    if flag=="withD" then D=coeff(Ds,0);Ds=Ds-D;end
    [N,B1,C1]=pol2des(Ds);
    [n1,n1]=size(N);
    [n2,n2]=size(A2);
    E=[N,0*ones(n1,n2);0*ones(n2,n1),eye(n2,n2)];
    A=[eye(N),0*ones(n1,n2);0*ones(n2,n1),A2];
    C=[C1,C2];
    B=[B1;B2];
    if flag<>"withD" then D=0*C*B;end
    S=tlist(["des","A","B","C","D","E"],A,B,C,D,E);
endfunction
