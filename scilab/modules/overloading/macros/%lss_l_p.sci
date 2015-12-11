// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [sr]=%lss_l_p(s,p)
    //sr=%lss_l_p(s,p) <=> sr=s\p
    // p polynomial matrix
    // s syslin list
    //!
    sr=s\tlist(["lss","A","B","C","D","X0","dt"],[],[],[],p,[],[])
endfunction
