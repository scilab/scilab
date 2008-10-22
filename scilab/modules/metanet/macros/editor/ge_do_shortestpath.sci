
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_shortestpath(GraphList)

  drawlater()
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;hnodes=gindex.node;

  k1=[]
  while k1==[]
    [btn,xc,yc]=xclick()
    if btn==2 then return,end
    k1=ge_getnode([xc;yc])
  end
  
  e=hnodes(k1); 
  e.children(2).thickness=3*ge_get_node_border(k1);
  draw(e);show_pixmap()


  k2=[]
  while k2==[]
    [btn,xc,yc]=xclick()
    if btn==2 then return,end
    k2=ge_getnode([xc;yc])
  end
  e=hnodes(k2); 
  e.children(2).thickness=3*ge_get_node_border(k2)
  draw(e);show_pixmap()

  
  l1=list('shortest path type',1,['Number of arcs',edgedatafields(GraphList)])
  l2=list('colors',5,string(1:xget("lastpattern")))
  rep=x_choices("Select shortest path type and highlighting color ",list(l1,l2))
  if rep==[] then return,end
  if rep(1)==1 then typ='arc',else typ='length',end
  [p,lp] = shortest_path(k1,k2,GraphList,typ)
  for k=p
    e=hedges(k);
    e.children(2).foreground=rep(2)
    e.children(3).foreground=rep(2)
  end
  drawnow();show_pixmap()

endfunction
