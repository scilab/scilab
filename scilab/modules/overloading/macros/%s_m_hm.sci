// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M2=%s_m_hm(M1,M2)
if size(M1,'*')<>1 then
  M2=hypermat(size(M1),M1)*M2
else
  M2.entries=M1*M2.entries
end
endfunction
