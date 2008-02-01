function ge_draw_graph(GraphList)
  drawlater()
  
  ge_axes_handle=gca();
  if typeof(ge_axes_handle.user_data)<>' gindex' then
    set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))
  end

  gindex=ge_axes_handle.user_data
  
  hnodes=[];
  for k=1:size(GraphList.nodes.graphics.x,'*'), 
    hnodes(k)=ge_draw_node(k); 
    if modulo(k,40)==0 then drawnow();show_pixmap();drawlater(),end
  end
  hedges=[];
  for k=1:size(GraphList.edges.tail,'*'), 
    hedges(k)=ge_draw_edge(k),
    if modulo(k,40)==0 then drawnow();show_pixmap();drawlater(),end
  end
  
  gindex.node=hnodes;gindex.edge=hedges;
  ge_axes_handle.user_data=gindex;
  
  drawnow();show_pixmap()
endfunction
