// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [sr]=%s_r_lss(p,s)
    // sr=%s_r_lss(p,s) ou sr=p/s
    // s : syslin list
    // p : constant matrix
    //!
    sr=tlist(["lss","A","B","C","D","X0","dt"],[],[],[],p,[],[])/s
endfunction
