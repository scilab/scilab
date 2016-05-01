// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


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
