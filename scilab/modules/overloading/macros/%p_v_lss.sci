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

function [sr]=%p_v_lss(p,s)
    //  feedback sr=(eye+p*s)\p
    //s=%p_v_lss(p,s) <=> sr=p/.s
    // p : polynomial matrix
    // s : state-space syslin list
    //!

    sr=tlist(["lss","A","B","C","D","X0","dt"],[],[],[],p,[],[])/.s
endfunction
