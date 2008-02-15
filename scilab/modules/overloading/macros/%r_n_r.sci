// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%r_n_r(l1,l2)
//%r_n_r(l1,l2) <=> l1<>l2 for rational matrices
//!

r=l1('dt')<>l2('dt')
if ~r then
  r=l1('num')<>l2('num')|(l1('den')<>l2('den')&l1('num')<>0)
end
endfunction
