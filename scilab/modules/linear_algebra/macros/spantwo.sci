
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

function [Xp,dima,dimb,dim]=spantwo(A,B)
    //Given two matrices A and B with same number of rows,
    //returns a square matrix Xp (not necessarily orthogonal)
    //such that :
    //         [*,0]    (dim-dimb rows)
    //Xp*[A,B]=[*,*]    (dima+dimb-dim rows)
    //         [0,*]    (dim-dima rows)
    //         [0,0]
    //The  dima first columns of inv(Xp) span range(A).
    //Columns dim-dimb+1 to dima of inv(Xp) span the intesection of
    //range(A) and range(B).
    //The dim first columns of inv(Xp) span range(A)+range(B).
    // Ex: A=[1,0,0,4;
    //        5,6,7,8;
    //        0,0,11,12;
    //        0,0,0,16];
    //B=[1,2,0,0]';C=[4,0,0,1];
    //Sl=ss2ss(syslin('c',A,B,C),rand(A));
    //[no,X]=contr(Sl(2),Sl(3));CO=X(:,1:no);  //Controllable part
    //[uo,Y]=unobs(Sl(2),Sl(4));UO=Y(:,1:uo);  //Unobservable part
    //[Xp,dimc,dimu,dim]=spantwo(CO,UO);    //Kalman decomposition
    //Slcan=ss2ss(Sl,inv(Xp));

    [X1,dim,dima]=spanplus(A,B);Xp=X1';
    B1B2B3=Xp*B;B1B2B3=B1B2B3(1:dim,:);
    [W,dimb]=rowcomp(B1B2B3);W=W(dim:-1:1,:);
    W11=W(1:dima,1:dima);W21=W(dima+1:dim,1:dima);
    if rcond(W11)<1.d-10 then
        //   Which is better?
        B1B2=B1B2B3(1:dima,:);[W,dimb0]=rowcomp(B1B2);W=W(dima:-1:1,:);
        [n1,n2]=size(A);
        Xp=sysdiag(W,eye(n1-dima,n1-dima))*Xp;
        return;
    end
    Q=[eye(dima,dima),zeros(dima,dim-dima);
    -W21*inv(W11),eye(dim-dima,dim-dima)];
    Xp(1:dim,:)=Q*W*Xp(1:dim,:);
endfunction
