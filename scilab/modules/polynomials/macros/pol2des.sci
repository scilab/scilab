// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [N,B,C]=pol2des(Ds)
    // Given the polynomial matrix Ds= D_0 +D_1 s +D_2 s^2 + ... +D_k s^k,
    // pol2des returns three  matrices N,B,C  (with N nilpotent) such that
    // Ds = C (sN-Eye)^-1 B
    //!
    if type(Ds)==1 then Ds=Ds+0*poly(0,"s")*Ds;end
    dg=max(degree(Ds))+1;
    [nout,nin]=size(Ds);
    [Sl]=markp2ss(coeff(Ds),dg,nout,nin);
    N=Sl(2);B=-Sl(3);C=Sl(4)
endfunction
