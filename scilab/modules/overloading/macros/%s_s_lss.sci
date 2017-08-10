// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=%s_s_lss(d1,s2)
    //s=d1-s2
    [a2,b2,c2,d2,x2,dom2]=s2(2:7),
    s=tlist(["lss","A","B","C","D","X0","dt"],a2,-b2,c2,d1-d2,x2,dom2),
endfunction
