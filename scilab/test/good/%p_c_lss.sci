// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [s]=%p_c_lss(d1,s2)
//s=%p_c_lss(d1,s2) <=>  s=[d1,s2]  d1: polynomial mat.  s2:syslin list
//!
[a2,b2,c2,d2,x2,dom2]=s2(2:7)
[n2,m2]=size(b2);[p1,m1]=size(d1)
s=tlist(['lss','A','B','C','D','X0','dt'],a2,[0*ones(n2,m1),b2],c2,[d1,d2],x2,dom2)
endfunction
