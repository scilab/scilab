function GraphList=ge_complete_defaults(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

  
  if GraphList.default_node_diam==[] then
    GraphList.default_node_diam=15
  end
  if GraphList.default_node_border==[]
    GraphList.default_node_border=1
  end
  if GraphList.default_font_size==[] then
    GraphList.default_font_size=8
  end
  if GraphList.default_edge_width==[] then
    GraphList.default_edge_width=1
  end
  
  nn=size(GraphList.node_x,'*')
  na=size(GraphList.tail,'*')
  if GraphList.node_border==[] then
    GraphList.node_border=zeros(1,nn)
  end
  if GraphList.node_color==[] then
    GraphList.node_color=ones(1,nn)
  end
  if GraphList.node_type==[] then
    GraphList.node_type=zeros(1,nn)
  end

  if GraphList.node_diam==[] then
    GraphList.node_diam=zeros(1,nn)
  end
  if GraphList.node_font_size==[] then
    GraphList.node_font_size=zeros(1,nn)
  end
  if GraphList.node_demand==[] then
    GraphList.node_demand=zeros(1,nn)
  end  
  if GraphList.node_label==[] then
    GraphList.node_label=emptystr(1,nn)
  end
  if GraphList.edge_label==[] then
    GraphList.edge_label=emptystr(GraphList.tail)
  end
  if GraphList.edge_name==[] then
    GraphList.edge_name=string(1:na)
  end
  if GraphList.edge_color==[] then
    GraphList.edge_color=ones(1,na)
  end
  if GraphList.edge_width==[] then
    GraphList.edge_width=ones(1,na)
  end
  if GraphList.edge_hi_width==[] then
    GraphList.edge_hi_width=ones(1,na)
  end
  if GraphList.edge_font_size==[] then
    GraphList.edge_font_size=zeros(1,na)
  end

endfunction
