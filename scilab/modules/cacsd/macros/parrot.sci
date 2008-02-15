// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
gamaopt=maxi( maxi(svd(D1)),maxi(svd(D2)) );
Kopt=-D22-D21*inv(gamaopt*gamaopt*eye()-D11'*D11)*D11'*D12;
endfunction
