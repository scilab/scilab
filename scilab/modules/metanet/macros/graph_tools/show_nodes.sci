
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function show_nodes(p,sup,leg)
  if exists('leg','local')==0 then leg='nothing',end
  if exists('sup','local')==0 then sup='no',end
  if type(p)<>1 then error(52,1),end
  sup=sup=='sup'
  //get current editgraph window
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current edit_graph window defined, use netwindow')
  end
  
  win=EGcurrent;w=string(win)
  old=gcf();scf(win)
  execstr(['global EGdata_'+w
	   '  EGdata=EGdata_'+w])
  if typeof(EGdata)=='egdata' then
    GraphList=EGdata.GraphList
    p=unique(p)
    if min(p)<1|max(p)>node_number(GraphList) then
      error('Node numbers should be in [1 '+string(node_number(GraphList))+']')
    end

    df=getfield(1,GraphList.nodes.data)
    NodeDataFields=['nothing','number','name', df(2:$)];
    if and(stripblanks(leg)<>NodeDataFields) then
      error('Invalid data field')
    end
    GraphList.nodes.graphics.display=leg //NodeId
    if ~sup then 
      ge_do_replot(GraphList)
    end
    ge_hilite_nodes(p,GraphList)
    show_pixmap()
  end
  scf(old)
endfunction
