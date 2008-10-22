// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%s_g_s(a,b)
//computes a|b for a and b matrices of scalar

if a==[]|b==[] then 
  r=[]
else
  r=(a<>0)|(b<>0)
end
endfunction
