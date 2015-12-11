
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [P11,P12,P21,P22]=fourplan(P,r)
    //returns the four plants associated with the augmented plant P.
    // r is the size of P22 and P=[P11,P12;P21,P22].

    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
    dom=P(7);
    P11=syslin(dom,A,B1,C1,D11);
    P12=syslin(dom,A,B2,C1,D12);
    P21=syslin(dom,A,B1,C2,D21);
    P22=syslin(dom,A,B2,C2,D22);
endfunction
