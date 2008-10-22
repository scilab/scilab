
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_draw_graph(GraphList)
  drawlater()
  
  ge_axes_handle=gca();
  if typeof(ge_axes_handle.user_data)<>' gindex' then
    set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))
  end

  gindex=ge_axes_handle.user_data
  
  hnodes=[];
  // all the constructed objects are filled
  a = gca();
  a.fill_mode = "on";
  for k=1:size(GraphList.nodes.graphics.x,'*'), 
    hnodes(k)=ge_draw_node(k); 
    //if modulo(k,40)==0 then drawnow();show_pixmap();drawlater(),end
  end
  a.fill_mode = "off";

  hedges=[];
  for k=1:size(GraphList.edges.tail,'*'), 
    hedges(k)=ge_draw_edge(k),
    //if modulo(k,40)==0 then drawnow();show_pixmap();drawlater(),end
  end
  
  gindex.node=hnodes;gindex.edge=hedges;
  ge_axes_handle.user_data=gindex;
  
  drawnow();show_pixmap();
endfunction
