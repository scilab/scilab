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

function f=%p_l_p(p2,p1)
    // f=%p_l_p(p2,p1) <=> f=p2\p1 for polynomial matrices
    //!
    [l,c]=size(p2)
    if l*c <>1 then f=invr(p2)*p1,return,end
    [l,c]=size(p1)
    [p1,p2]=simp(p1,p2*ones(l,c))
    f = rlist(p1,p2,[])
endfunction
