// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s2=%p_s_lss(d1,s2)
//s=%p_s_lss(s1,d2) <=> s=p-s1
// s1 : syslin list
// p  : polynomial matrix
//!

s2(5)=d1-s2(5)
endfunction
