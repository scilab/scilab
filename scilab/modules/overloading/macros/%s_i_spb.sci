// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%s_i_spb(varargin)

[lhs,rhs]=argn(0)
M=bool2s(varargin(rhs))
N=varargin(rhs-1)//inserted matrix
if rhs<=4 then
  if rhs==3 then
    M(varargin(1))=N
  else
    M(varargin(1),varargin(2))=N
  end
  return
end
  
endfunction
