// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=pixDist(p1,p2)
//computes the distance in pixels of two points
  [x1,y1]=xchange([p1(1) p2(1)],[p1(2) p2(2)],'f2i')
  d=max(abs([x1(2)-x1(1),y1(2)-y1(1)]))
endfunction
