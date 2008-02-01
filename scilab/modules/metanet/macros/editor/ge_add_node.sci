function GraphList=ge_add_node(GraphList,node_x,node_y)
//Copyright INRIA
//Author : Serge Steer 2002

  n=size(GraphList.node_x,'*')+1
  GraphList.node_name(1,n)=""
  GraphList.node_type(1,n)=0;
  GraphList.node_x(1,n)=node_x
  GraphList.node_y(1,n)=node_y
  GraphList.node_color(1,n)=1;
  GraphList.node_diam(1,n)=0
  GraphList.node_border(1,n)=0
  GraphList.node_font_size(1,n)=0
  GraphList.node_demand(1,n)=0;
  GraphList.node_label(1,n)='';
  ge_drawnodes(n) 
  ge_add_history(list('add_node',n))
endfunction
