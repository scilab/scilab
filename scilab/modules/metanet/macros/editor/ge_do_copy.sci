
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_copy(GraphList,xc,yc)
  k=ge_getnode(GraphList,[xc;yc])
  if k==[] then return,end
  
  rep(3)=-1
  dr=driver()
  x_save=GraphList.node_x(k)
  y_save=GraphList.node_y(k)
  if dr=='Rec' then driver('X11'),end
  ge_drawnodes(k) //erase
  if xget('pixmap') then xset('wshow'),end
  while rep(3)==-1 then //move loop
			// get new position
			rep=xgetmouse(0)
			ge_drawnodes(k) //redraw
			if xget('pixmap') then xset('wshow'),end

			GraphList.node_x(k)=rep(1);
			GraphList.node_y(k)=rep(2);
			ge_drawnodes(k) //redraw
			if xget('pixmap') then xset('wshow'),end
  end

  if rep(3)==2 then //cancel
    ge_drawnodes(k) //erase
    GraphList.node_x(k)=x_save
    GraphList.node_y(k)=y_save
    ge_drawnodes(k) 
    if xget('pixmap') then xset('wshow'),end
    driver(dr)
    return
  end
  //add
  ge_drawnodes(k) //erase
  GraphList=ge_add_node_copy(GraphList,k)
  driver(dr);ge_drawnodes(size(GraphList.node_x,'*')) //redraw and record
  edited=return(%t)

endfunction

