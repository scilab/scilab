// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function str=datatipDefaultDisplay(curve,pt,index)
  if size(pt,'*')==2 then
    str=msprintf('X: %.2g\nY: %.2g', pt(1),pt(2))
  else
    str=msprintf('X: %.2g\nY: %.2g\nZ: %.2g', pt(1),pt(2),pt(3))
  end
endfunction
