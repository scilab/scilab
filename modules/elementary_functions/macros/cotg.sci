// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t] = cotg(z)
// 
//  PURPOSE 
//     element wise cotangent
//
  if type(z)<>1 then error(53,1),end
  t = 1 ./tan(z)
endfunction
