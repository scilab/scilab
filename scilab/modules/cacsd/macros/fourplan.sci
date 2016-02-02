
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
