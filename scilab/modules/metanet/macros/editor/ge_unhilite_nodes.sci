function ge_unhilite_nodes(p,GraphList)
//Copyright INRIA
//Author : Serge Steer 2008
  f=gcf(); drawing=f.immediate_drawing;f.immediate_drawing='off'

  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data;
  hnodes=gindex.node;
  
  for k=1:size(p,'*')
    e=hnodes(p(k)); 
    e.children(2).thickness=ge_get_node_border(p(k))
  end
  f.immediate_drawing=drawing
  show_pixmap()
endfunction
