// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s1=%lss_s_p(s1,d2)
//s=%lss_s_p(s1,d2) <=> s=s1-p
// s1 : syslin list
// p  : polynomial matrix
// Copyright INRIA
s1(5)=s1(5)-d2
endfunction
