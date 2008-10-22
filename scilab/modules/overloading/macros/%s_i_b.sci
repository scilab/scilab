// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%s_i_b(varargin)

  rhs=argn(2)
  M=varargin($)
  if rhs<=4 then //type conversion
    M=bool2s(M)
    M(varargin(1:$-2))=varargin($-1)
  else //hypermatrix syntax
    if varargin($-1)<>[] then M=bool2s(M),end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
  end 
endfunction
