// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=coth(x)
//Syntax : t=coth(x)
//
// hyperbolic co-tangent of x

  if type(x)<>1 then error(53,1),end
  t=exp(x);
  t=(t-ones(x)./t).\(t+ones(x)./t)
endfunction
