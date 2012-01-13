// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) INRIA - 2011 - Serge STEER
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at  
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function varargout = %hm_size(M, job)

  // returns dimensions of an hyper matrix
  [lhs, rhs] = argn(0);
  dims = matrix(double(M.dims), 1, -1);
  if rhs == 2 then
    if type(job) == 10 then
      select job
      case '*' then
        varargout(1) = prod(dims);
      case 'r' then
        varargout(1) = dims(1);
      case 'c' then
        varargout(1) = dims(2);
      else
        error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"), "size", 2, "r", "c", "*"))
      end
      return
    elseif type(job) == 1 then
      if (size(job,'*') > 1) | (job <= 0)  then 
        error(msprintf(_("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "size", 2)); 
      end
      if job <= size(dims, '*') then
        varargout(1) = dims(job);
      else
        varargout(1) = 1;
      end
    else
      error(msprintf(_("%s: Wrong value for input argument #%d: Scalar positive integer or character string expected.\n"),"size", 2));
    end
    return
  end
  if lhs == 1 then
    varargout(1) = matrix(dims, 1, -1);
  else
    if lhs > size(dims, '*') then
      dims($+1:lhs) = 1;
    end
    for k = 1:lhs
      varargout(k) = dims(k);
    end
  end
endfunction
