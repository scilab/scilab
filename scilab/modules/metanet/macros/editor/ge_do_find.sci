
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_find(key,GraphList)
  
//hauteur de la  barre de menus used because figure_size is the
//size of the graphic window, not the size of the visiblze part
//of the drawable
  hmen=60;
  ge_win_handle=gcf();
  wpd=ge_win_handle.figure_size;wpd(2)=wpd(2)-hmen;

  select key
   case "Node" then
    while %t
      ok=%t
      r=x_dialog('Enter a Node number or a Node name:','')
      if r==[] then return,end
      r=stripblanks(r);
      r(r=='')=[]
      if size(r,'*')>1 then
	messagebox('Only one node should be selected:'+r,"modal","error");
	ok=%f
      else
	ierr=execstr('n='+r,'errcatch')
	if ierr==0 then // a node number
	  if n<1|n>size(GraphList.nodes) then
	    messagebox('Incorrect node number:'+r,"modal","error");
	    ok=%f
	  end
	else
	  n=find(GraphList.nodes.graphics.name==r)
	  if n==[] then
	    messagebox('Unknown node :'+r,"modal","error");
	    ok=%f
	  end
	end
      end
      if ok then break,end
    end
    [x,y]=xchange(GraphList.nodes.graphics.x(n),GraphList.nodes.graphics.y(n),"f2i") ;
    ge_set_viewport(x-wpd(1)/2,y-wpd(2)/2)
    ge_hilite_nodes(n,GraphList)
   case "Arc" then
    while %t
      ok=%t
      r=x_dialog('Enter an Arc number or an Arc name:','')
      if r==[] then return,end
      r=stripblanks(r);
      r(r=='')=[]
      if size(r,'*')>1 then
	    messagebox('Only one arc should be selected:'+r,"modal","error");
	    ok=%f
      else
	ierr=execstr('n='+r,'errcatch')
	if ierr==0 then // a node number
	  if n<1|n>size(GraphList.edges) then
	    messagebox('Incorrect arc number:'+r,"modal","error");
	    ok=%f
	  end
	else
	  n=find(GraphList.edges.graphics.name==r)
	  if n==[] then
	    messagebox('Unknown arc :'+r,"modal","error");
	    ok=%f
	  end
	end
      end
      if ok then break, end
    end
    t=GraphList.edges.tail(n)
    h=GraphList.edges.head(n)
    [x,y]=xchange(sum(GraphList.nodes.graphics.x([t h]))/2,sum(GraphList.nodes.graphics.y([t h]))/2,"f2i") ;
    ge_set_viewport(x-wpd(1)/2,y-wpd(2)/2)
    ge_hilite_edges(n,GraphList) 
  end
endfunction


