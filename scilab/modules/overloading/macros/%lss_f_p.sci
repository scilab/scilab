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

function [s]=%lss_f_p(s1,d2)
    //[s]=%lss_f_p(s1,d2) <=> s=[s1;d2]
    //!
    //
    [a1,b1,c1,d1,x1,dom1]=s1(2:7)
    [n1,m1]=size(c1);[p2,m2]=size(d2);
    s=tlist(["lss","A","B","C","D","X0","dt"],a1,b1,[c1;0*ones(p2,m1)],[d1;d2],x1,dom1)
endfunction
