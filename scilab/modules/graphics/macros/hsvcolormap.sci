// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=hsvcolormap(n)
  if n<1 then error('hsvcolormap : n must be an integer greater than 1'),end
  c = hsv2rgb([(0:n-1)'/n ones(n,2)]);
endfunction
