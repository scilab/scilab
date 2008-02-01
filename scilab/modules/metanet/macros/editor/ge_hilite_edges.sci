function ge_hilite_edges(p,GraphList)
//Copyright INRIA
//Author : Serge Steer 2002
  f=gcf(); drawing=f.immediate_drawing;f.immediate_drawing='off'
  ge_axes_handle=gca();gindex=ge_axes_handle.user_data; hedges=gindex.edge;

  for k=1:size(p,'*')
    e=hedges(p(k)); 
    e.children(3).thickness=3*ge_get_edge_style(p(k));
  end
  f.immediate_drawing=drawing
  show_pixmap()

endfunction
