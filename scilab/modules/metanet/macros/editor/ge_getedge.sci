
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function k=ge_getedge(GraphList,pt)

  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;
  found=%f
  for k=1:size(hedges,'*')
    ak=get(hedges(k),'children');ak=ak(3)
    xy=get(ak,'data');//path
    if get(ak,'type')=='Polyline' then
      if ge_dist2polyline(xy(:,1),xy(:,2),pt) < 10 then return,end
    else
      if ge_dist2edge(xy,pt) < 10 then return,end
    end
  end
  k=[]
endfunction


