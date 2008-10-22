// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=st_i_generic(varargin)
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //
  if M==[] then M=struct(),M(index)=N;return;end
  if rhs==3&(type(index)==10|type(index)==15) then
    if type(index)==15 then
      M=struct()
      M(index)=N
    else
      M=createstruct(index,N)
    end
  else
    error(1)
  end

endfunction
