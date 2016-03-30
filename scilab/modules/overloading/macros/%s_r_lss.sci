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

function [sr]=%s_r_lss(p,s)
    // sr=%s_r_lss(p,s) ou sr=p/s
    // s : syslin list
    // p : constant matrix
    //!
    sr=tlist(["lss","A","B","C","D","X0","dt"],[],[],[],p,[],[])/s
endfunction
