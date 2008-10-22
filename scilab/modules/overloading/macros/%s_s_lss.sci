// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s=%s_s_lss(d1,s2)
//s=d1-s2
[a2,b2,c2,d2,x2,dom2]=s2(2:7),
s=tlist(['lss','A','B','C','D','X0','dt'],a2,-b2,c2,d1-d2,x2,dom2),
endfunction
