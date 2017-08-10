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

function [Kopt,gamaopt]=parrot(D,r)
    //Given a matrix D partionned as [D11 D12
    //                                D21 D22]
    //where size(D22)=r=[r1,r2]
    //compute a matrix K such that
    //largest singular value of [D11 D12
    //                           D21 D22+K]
    //is minimal (Parrot's theorem)
    //!

    [l,k]=size(D);
    l1=1:(l-r(1));
    l2=(l-r(1)+1):l;
    k1=1:(k-r(2))
    k2=(k-r(2)+1):k;
    D11=D(l1,k1);
    D12=D(l1,k2);
    D21=D(l2,k1);
    D22=D(l2,k2);
    D1=[D11,D12];D2=[D11;D21];
    gamaopt=max( max(svd(D1)),max(svd(D2)) );
    Kopt=-D22-D21*inv(gamaopt*gamaopt*eye()-D11'*D11)*D11'*D12;
endfunction
