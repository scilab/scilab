// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%hm_n_b(M,s)
//M<>s
  if size(s,'*')<> 1 then
    M=%t;
  else
    M.entries=M.entries<>s
  end
endfunction
