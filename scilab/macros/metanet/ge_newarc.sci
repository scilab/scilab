function GraphList=ge_newarc(GraphList,xc1,yc1)
//Copyright INRIA
//Author : Serge Steer 2002

//edition of a link from an output block to an input  block
// Copyright INRIA
  dash=xget('dashes')

  kfrom=ge_getnode(GraphList,[xc1;yc1])

  if kfrom==[] then return,end

  xl=GraphList.node_x(kfrom)
  yl=GraphList.node_y(kfrom)

  clr=1
  xset('dashes',clr)
  dr=driver()
  if dr=='Rec' then driver('X11'),end
  xe=xl;ye=yl
  while %t do //wait for a node selection
    xpoly([xl;xe],[yl;ye],'lines')
    rep(3)=-1
    while rep(3)<0 do //get a new point
      rep=xgetmouse()
      
      //erase last link segment
      xpoly([xl;xe],[yl;ye],'lines')
      //plot new position of last link segment
      xe=rep(1);ye=rep(2)
      xpoly([xl;xe],[yl;ye],'lines')
      if xget('pixmap') then xset('wshow'),end
    end
    if rep(3)==2 then // abort
      xpoly([xl;xe],[yl;ye],'lines')
      if xget('pixmap') then xset('wshow'),end
      driver(dr)
      return
    end
    kto=ge_getnode(GraphList,rep(1:2))
    if kto<>[] then // a destination node selected
      break
    else
      xpoly([xl;xe],[yl;ye],'lines')
      if xget('pixmap') then xset('wshow'),end
    end
  end

  xpoly([xl;xe],[yl;ye],'lines')
  if xget('pixmap') then xset('wshow'),end

  sel=find((GraphList.head==kfrom&GraphList.tail==kto)|..
	   (GraphList.head==kto&GraphList.tail==kfrom))
  driver(dr)
  if sel<>[] then ge_drawarcs(sel),end //erase
  
  GraphList=ge_add_arc(GraphList,head=kto,tail=kfrom)
  sel($+1)=size(GraphList.head,'*')
  ge_drawarcs(sel)
  edited=resume(%t)
endfunction
