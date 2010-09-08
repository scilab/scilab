// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - E. Segre
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function s=%b_string(b)
// thanks to E. Segre for this efficient implementation
  a=["F" "T"]
  s=matrix(a(1+b),size(b))
endfunction	

