function ge_unhilite_edges(p,GraphList)
//Copyright INRIA
//Author : Serge Steer 2008
  f=gcf(); drawing=f.immediate_drawing;f.immediate_drawing='off'
  ge_axes_handle=gca();gindex=ge_axes_handle.user_data; hedges=gindex.edge;

  for k=1:size(p,'*')
    e=hedges(p(k)); 
    e.children(3).thickness=ge_get_edge_style(p(k));
  end
  f.immediate_drawing=drawing
  show_pixmap()

endfunction
