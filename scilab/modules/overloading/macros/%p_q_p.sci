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

function f=%p_q_p(p1,p2)
    //f= p1.\p2
    //!

    if size(p1,"*")==1 then
        p1=p1*ones(p2)
    elseif size(p2,"*")==1 then
        p2=p2*ones(p1)
    end
    [p1,p2]=simp(p1,p2)
    f = rlist(p2,p1,[])
endfunction
