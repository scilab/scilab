
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function ge_drawobjs(GraphList,nodesel,arcsel)
  
  if exists('nodesel','local')==0 then nodesel=:,end
  if exists('arcsel','local')==0 then arcsel=:,end
  
  ge_drawtitle(GraphList.name)
  ge_drawarcs(arcsel)
  ge_drawnodes(nodesel)
endfunction

