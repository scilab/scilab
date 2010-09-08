// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s2=%r_i_lss(i,j,s1,s2)
//%r_i_lss(i,j,s1,s2) <=> s2(i,j)=s1
//!
if type(i)==10|type(j)==10 then 
  error(21)
end
[s1 s2]=sysconv(s1,s2)
s2(i,j)=s1
endfunction
