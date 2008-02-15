// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%l_isequal(x1,varargin)
  narg=size(varargin)
  if narg==0 then error('isequal requires at least two input arguments'),end

  r=%t
  m=lstsize(x1)

  for k=1:narg
   n=lstsize(varargin(k))
   r=r&(n==m)
   if ~r then return,end
  end

  for k=1:narg
    for l=1:m
      r=r&isequal(getfield(l,x1),getfield(l,varargin(k)))
      if ~r then return,end
    end
  end
endfunction

