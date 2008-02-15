
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xyp=ge_get_edge_profile(n)
  prof=GraphList.edges.graphics.profile_index
  if prof==[] then
    index=0
  else
    index=GraphList.edges.graphics.profile_index(n)
  end
  if index==0 then
    xyp=ge_default_edge_profile()
  else
    xyp=GraphList.edges.graphics.profiles(index)
  end
endfunction
