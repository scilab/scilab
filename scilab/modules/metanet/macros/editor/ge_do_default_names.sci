
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_default_names(GraphList)
  r=x_message(['This operation will replaces ALL node and arc names'
	       'by their default values.'
	       'Do you really want to do it?'],['yes','no'])
  if r==1 then
    
    GraphList.nodes.graphics.name=string(1:GraphList.nodes.number)
    GraphList.edges.graphics.name=string(1:size(GraphList.edges.head,'*'))

    NodeId=GraphList.nodes.graphics.display;
    ArcId=GraphList.edges.graphics.display;
    ge_axes_handle=gca();
    gindex=ge_axes_handle.user_data
    drawlater()
    if ArcId==2 then
      hedges=gindex.edge;
      for k=1:size(hedges,'*')
	e=hedges(k)
	e.children(1).text=string(k)
      end
    end
    if NodeId==2 then
      hnodes=gindex.node;
      for k=1:size(hnodes,'*')
	e=hnodes(k)
	e.children(1).text=string(k)
      end
    end
    drawnow();show_pixmap()
  end
endfunction
