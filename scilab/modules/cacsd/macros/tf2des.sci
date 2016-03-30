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

function [S]=tf2des(G,flag)
    //[S]=tf2des(G)
    // Transfer function to descriptor form: S=list('d',A,B,C,D,E)
    //  E*dx=A*x+B*u
    //  y=C*x+D*u
    //  with flag="withD" a maximal rank D matrix is returned
    //!

    [LHS,RHS]=argn(0);
    if RHS==1 then flag=[];end
    if RHS==2&flag<>"withD" then warning(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"tf2des",2,"withD"));end
    Num=G(2);Den=G(3);
    %s=poly(0,varn(Den));
    [n,m]=size(Num);
    pol=zeros(n,m);pro=pol;
    //        Pro = strictly proper part of G
    //        Pol = polynomial part of G.
    for l=1:n,
        for k=1:m,
            denlk=Den(l,k);
            [r,q]=pdiv(Num(l,k)+0*%s,denlk+0*%s);
            pol(l,k)=q;
            pro(l,k)=r/denlk;
        end;
    end;

    sp=tf2ss(pro);
    D=zeros(Num);

    if flag=="withD" then
        D=coeff(pol,0);pol=pol-D;
    end;
    spol=tf2ss(horner(pol,1/%s)/%s);

    [n1,n1]=size(sp(2));
    [n2,n2]=size(spol(2));
    A=[sp(2),0*ones(n1,n2);
    0*ones(n2,n1),eye(n2,n2)];
    E=[eye(n1,n1),0*ones(n1,n2);
    0*ones(n2,n1),spol(2)];
    B=[sp(3);
    spol(3)];
    C=[sp(4),-spol(4)];
    S=tlist(["des","A","B","C","D","E"],A,B,C,D,E)
endfunction
