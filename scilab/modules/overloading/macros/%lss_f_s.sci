// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [s]=%lss_f_s(s1,d2)
    //operation  s=[s1;d2]
    //!
    [a1,b1,c1,d1,x1,dom1]=s1(2:7)
    [n1,m1]=size(c1);[p2,m2]=size(d2);
    s=tlist(["lss","A","B","C","D","X0","dt"],a1,b1,[c1;0*ones(p2,m1)],[d1;d2],x1,dom1)
endfunction
