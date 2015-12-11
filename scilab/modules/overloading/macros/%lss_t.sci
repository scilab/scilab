// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s=%lss_t(s)
    //s=%lss_t(s)  <=> s=s'  in state-space
    //!

    [a,b,c,d,x,dom]=s(2:7)
    s=tlist(["lss","A","B","C","D","X0","dt"],a',c',b',d',x,dom)
endfunction
