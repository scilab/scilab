function ge_drawarcs(sel)
//Copyright INRIA
//Author : Serge Steer 2002

  arrowWidth=6;arrowLength=6;
  if or(type(sel)==[2 129]) then 
    sel=horner(sel,size(GraphList.tail,'*')),
  elseif size(sel,1)==-1 then 
    sel=1:size(GraphList.tail,'*'),
  end
  if GraphList.tail(sel)==[] then return,end
 
  [xx,yy,sel1,loops]=ge_arc_coordinates(sel)
  
  ge_draw_std_arcs(xx,yy,ge_get_arcs_id(sel1))
  

  if loops<>[] then
    loops=sel(loops)
    ge_draw_loop_arcs(loops)
  end

  if xget('pixmap') then xset('wshow'),end
endfunction

