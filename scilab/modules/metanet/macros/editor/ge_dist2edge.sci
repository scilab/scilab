
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=ge_dist2edge(arc,pt)
  rx=arc(3)/2;ry=arc(4)/2;
  x1=arc(1)+rx;
  y1=arc(2)-ry;
  t=atan(pt(2)-y1,pt(1)-x1);
  d=sqrt((pt(1)-x1-rx*cos(t))^2+(pt(2)-y1-ry*sin(t))^2)
endfunction

