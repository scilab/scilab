// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Kopt,gamaopt]=h_inf_st(D,r)
    //compute a matrix K such that largest singular value of
    //       Fl(D,r,K)=D11+D12 K inv(I-D22 K) D21
    //is minimal (Static H-infinity four blocks problem)
    //
    //[Kopt,gamaopt]=h_inf_st(D,r)
    //
    //  D   partionned as [D11 D12
    //                     D21 D22]
    //where size(D22)=r=[r1 r2]


    [l,k]=size(D);
    l1=1:(l-r(1));
    l2=(l-r(1)+1):l;
    k1=1:(k-r(2))
    k2=(k-r(2)+1):k;
    D11=D(l1,k1);
    D12=D(l1,k2);
    D21=D(l2,k1);
    D22=D(l2,k2);
    [U,n]=rowcomp(D12);
    //n=r(2) ?
    if n<>r(2) then mprintf(gettext("%s: %s not full rank.\n"),"h_inf_st","D12"),end
    U=U([n+1:l-r(1),1:n],:);       //Bottom Compression
    [V,m]=colcomp(D21);
    //m=r(1) ?
    if m<>r(1) then mprintf(gettext("%s: %s not full rank.\n"),"h_inf_st","D21"),end
    //   Update
    D12=U*D12;
    D11=U*D11*V;
    D21=D21*V;
    [Kinf,gamaopt]=parrot(D11,[n,m])
    D12c=D12(l-r(1)-n+1:l-r(1),:);
    D21c=D21(:,k-r(2)-m+1:k-r(2));
    K#=D12c\Kinf/D21c;
    Kopt=(eye()+K#*D22)\K#
endfunction
