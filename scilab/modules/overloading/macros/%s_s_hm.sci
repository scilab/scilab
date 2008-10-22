// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA/ENPC
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M2=%s_s_hm(M1,M2)

// scalar -hypermatrix 
  if and(size(M1)==[1 1]) then
    M2.entries=M1-M2.entries
  else
    dims1=size(M1)';
    dims2=matrix(M2.dims,-1,1)
    dims1(3:size(dims2,'*'))=1
    if and(dims1==dims2) then //should not occur
       M2.entries=matrix(M1,-1,1)-M2.entries
     else
       error(9)
     end
  end
endfunction
