function GraphList=ge_do_properties(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  edited=%f

  [k,wh]=ge_getobj(GraphList,[xc;yc])
  if k==[] then return,end
  if wh=='node' then 
    fn=ge_node_fields()
    typ=list();ini=[];
    for f=fn
      x=GraphList(f)(k)
      if type(x)==1 then
	typ($+1)='vec'; typ($+1)=1
	ini=[ini sci2exp(x)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
    execstr('[ok,'+strcat(fn,',')+']=getvalue(''Enter Node properties'',fn,typ,ini)')
    if ~ok then return,end
    Hist=list("node_property",k)
    for f=fn
      if GraphList(f)(k)<>evstr(f) then
	edited=%t,
	Hist($+1)=f;Hist($+1)=GraphList(f)(k);
      end
    end
    if edited
      karcs=find(GraphList.tail==k|GraphList.head==k)
      ge_drawarcs(karcs);ge_drawnodes(k); //erase arcs and node
      for f=fn
	GraphList(f)(k)=evstr(f);
      end
      ge_drawnodes(k);ge_drawarcs(karcs) //redraw
      ge_add_history(Hist)
    end
  else
    fn=ge_arc_fields()
    fn(fn=='head')=[]
    fn(fn=='tail')=[]
    
    typ=list();ini=[];
    for f=fn
      x=GraphList(f)(k)
      if type(x)==1 then
	typ($+1)='vec'; typ($+1)=1
	ini=[ini sci2exp(x)]
      elseif type(x)==10 then
	typ($+1)='str'; typ($+1)=-1
	ini=[ini x]
      else
      end
    end
    execstr('[ok,'+strcat(fn,',')+']=getvalue(''Enter edge properties'',fn,typ,ini)')
    if ~ok then return,end
    Hist=list("edge_property",k)
    for f=fn
      if GraphList(f)(k)<>evstr(f) then 
	edited=%t,
	Hist($+1)=f;Hist($+1)=GraphList(f)(k);
      end
    end
    if edited then
      T=GraphList.tail(k);H=GraphList.head(k)
      karcs=find((GraphList.tail==T&GraphList.head==H)|(GraphList.tail==H&GraphList.head==T))
      ge_drawarcs(karcs) //erase arc
      for f=fn
	GraphList(f)(k)=evstr(f);
      end
      ge_drawarcs(karcs) //redraw
      ge_add_history(Hist)
    end
  end
  edited=return(edited)
endfunction

