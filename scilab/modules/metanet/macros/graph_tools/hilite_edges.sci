
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function hilite_edges(p)
  //get current editgraph window
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current edit_graph window defined, use netwindow')
  end
  win=EGcurrent;w=string(win)
  old=gcf();scf(win)
  execstr(['global EGdata_'+w
	   'EGdata=EGdata_'+w])
  if typeof(EGdata)=='egdata' then
    GraphList=EGdata.GraphList
    p=unique(p)
    if min(p)<1|max(p)>edge_number(GraphList) then
      error('Edge numbers should be in [1 '+string(edge_number(GraphList))+']')
    end
    ge_hilite_edges(p,GraphList)
    show_pixmap()
  end
  scf(old)
endfunction

