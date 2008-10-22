
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_properties(GraphList,xc,yc)

  edited=%f
  [k,wh]=ge_getobj(GraphList,[xc;yc])
  if k==[] then return,end

  if wh=='node' then 
    node=GraphList.nodes(k)
    //graphic fields
    fn=getfield(1,node.graphics);fn=fn(2:$)
    fn(fn=='defaults'|fn=='display')=[];
    //data fields
    fd=getfield(1,node.data);fd=fd(2:$)
    if fn==[]&fd==[] then return,end
    typ=list();ini=[];
    for f=fn,
      x=node.graphics(f)
      if type(x)==1 then
	typ($+1)='mat'; typ($+1)=size(x)
	ini=[ini sci2exp(x,0)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
 
    for f=fd
      x=node.data(f)
      if type(x)==1 then
	typ($+1)='mat'; typ($+1)=size(x)
	ini=[ini sci2exp(x,0)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
    execstr('[ok,'+strcat('%'+[fn fd],',')+']=getvalue(''Enter Node properties'',[fn,fd],typ,ini)')
    if ~ok then return,end

    Hist=list("node_property",k)

    //diameter or coordinates change implies to rebuild connected arcs
    rebuild_edge= node.graphics.diam<>%diam|node.graphics.x<>%x|node.graphics.y<>%y
    for f=fn
      execstr('val=%'+f)
      if or(node.graphics(f)<>val) then
	edited=%t,
	Hist($+1)=['graphics',f];Hist($+1)=node.graphics(f);
	node.graphics(f)=val
      end
    end
    for f=fd
      execstr('val=%'+f)
      if or(node.data(f)<>val) then
	edited=%t,
	Hist($+1)=['data',f];Hist($+1)=node.data(f);
	node.data(f)=val
      end
    end
    if edited
      GraphList.nodes(k)=node
      karcs=find(GraphList.edges.tail==k|GraphList.edges.head==k)
      drawlater()
      ge_axes_handle=gca()
      gindex=ge_axes_handle.user_data
      hedges=gindex.edge;hnodes=gindex.node;

      //delete the graphic entities associated with the node
      delete(hnodes(k))
      // recreate a node
      hnodes(k)=ge_draw_node(k)
      
      if rebuild_edge then
	karcs=find(GraphList.edges.tail==k|GraphList.edges.head==k)
	modified=hedges(karcs)
	ge_update_edges(karcs,modified)
      end
      
      ge_add_history(Hist)
    end
  else
    edge=GraphList.edges(k)
    //edge graphic fields
    fa=getfield(1,edge.graphics);fa=fa(2:$)
    fa(fa=='defaults'|fa=='profiles'|fa=='display')=  [];
  
    typ=list();ini=[];
    for f=fa
      x=edge.graphics(f)
      if type(x)==1 then
	typ($+1)='mat'; typ($+1)=size(x)
	ini=[ini sci2exp(x,0)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
    //edge data fields
    fd=getfield(1,edge.data);fd=fd(2:$)
    for f=fd
      x=edge.data(f)
      if type(x)==1 then
	typ($+1)='mat'; typ($+1)=size(x)
	ini=[ini sci2exp(x,0)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
    execstr('[ok,'+strcat('%'+[fa fd],',')+']=getvalue(''Enter edge properties'',[fa fd],typ,ini)')
    if ~ok then return,end
    Hist=list("edge_property",k)
    for f=fa
      execstr('val=%'+f)
      if or(edge.graphics(f)<>val) then 
	edited=%t,
	Hist($+1)=['graphics',f];Hist($+1)=edge.graphics(f);
	edge.graphics(f)=val
      end
    end
    for f=fd
      execstr('val=%'+f)
      if or(edge.data(f)<>val) then 
	edited=%t,
	Hist($+1)=['data',f];Hist($+1)=edge.data(f);
	edge.data(f)=val;
      end
    end
    
    if edited then
      GraphList.edges(k)=edge;
      drawlater()
      ge_axes_handle=gca()
      gindex=ge_axes_handle.user_data
      hedges=gindex.edge; hnodes=gindex.node;     

      delete(hedges(k))
      hedges(k)=ge_draw_edge(k)

      ge_add_history(Hist)
    end
  end
  if edited then
    gindex.edge=hedges;gindex.node=hnodes;
    ge_axes_handle.user_data=gindex
    drawnow();show_pixmap()
  end
  edited=return(edited)
endfunction

