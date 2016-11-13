// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=%s_c_lss(d1,s2)
    //  s=[d1,s2] d1 gain, s2 state-space
    //!
    // origin s. steer inria 1987
    //
    // Copyright INRIA
    [a2,b2,c2,d2,x2,dom2]=s2(2:7)
    [n2,m2]=size(b2);[p1,m1]=size(d1)
    s=tlist(["lss","A","B","C","D","X0","dt"],a2,[0*ones(n2,m1),b2],c2,..
    [d1,d2],x2,dom2)
endfunction
