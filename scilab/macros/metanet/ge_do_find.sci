function ge_do_find(key)
//Copyright INRIA
//Author : Serge Steer 2002
  
  select key
  case "Node" then
    r=x_dialog('Enter a Node number or a Node name:','')
    if r<>[] then
      ierr=execstr('n='+r,'errcatch')
      w=string(win)
      execstr('global EGdata_'+w+';GraphList=EGdata_'+w+'.GraphList')
      if ierr==0 then // a node number
	if n<1|n>size(GraphList.node_x,'*') then
	  x_message('Incorrect node number:'+r)
	end
      else
	n=find(GraphList.node_name==r)
	if n==[] then
	  x_message('Unknown node :'+r)
	  return
	end
	
      end
      [x,y]=xchange(GraphList.node_x(n),GraphList.node_y(n),"f2i") ;
      wd=xget("wdim");
      wpd=xget("wpdim");wpd(2)=wpd(2)-60;// -60 hauteur de la barre de menus
      xset("viewport",x-wpd(1)/2,y-wpd(2)/2)
      ge_hilite_nodes(n,GraphList)
    end
  case "Arc" then
    r=x_dialog('Enter an Arc number or an Arc name:','')
    if r<>[] then
      ierr=execstr('n='+r,'errcatch')
      w=string(win)
      execstr('global EGdata_'+w+';GraphList=EGdata_'+w+'.GraphList')
      if ierr==0 then // a node number
	if n<1|n>size(GraphList.tail,'*') then
	  x_message('Incorrect arc number:'+r)
	end
      else
	n=find(GraphList.edge_name==r)
	if n==[] then
	  x_message('Unknown arc :'+r)
	  return
	end
	
      end
      t=GraphList.tail(n)
      h=GraphList.head(n)
      [x,y]=xchange(sum(GraphList.node_x([t h]))/2,sum(GraphList.node_y([t h]))/2,"f2i") ;
      wd=xget("wdim");
      wpd=xget("wpdim");wpd(2)=wpd(2)-60;// -60 hauteur de la barre de menus
      xset("viewport",x-wpd(1)/2,y-wpd(2)/2)
      ge_hilite_arcs(n,GraphList) 
    end
  else
  end
endfunction


