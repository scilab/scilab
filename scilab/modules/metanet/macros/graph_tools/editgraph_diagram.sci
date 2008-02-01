function GraphList=editgraph_diagram(v1,v2)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  
  tt = ['graph','name','directed','node_number','tail','head',..
	'node_name','node_type','node_x',..
	'node_y','node_color','node_diam','node_border',..
	'node_font_size','node_demand','edge_name',..
	'edge_color','edge_width','edge_hi_width','edge_font_size',..
	'edge_length','edge_cost',..
	'edge_min_cap','edge_max_cap','edge_q_weight',..
	'edge_q_orig','edge_weight',..
	'default_node_diam','default_node_border','default_edge_width',..
	'default_edge_hi_width',..
	'default_font_size','node_label','edge_label']  
  
  if exists('name','local')==0 then name='Untitled',end
  directed=1
  nodenumber=0 //_ removed to avoid name conflict with function node_number
  tail=[]
  head=[]
  node_name=[]
  node_type=[]
  node_x=[]
  node_y=[]
  node_color=[]
  node_diam=[]
  node_border=[]
  node_font_size=[]
  node_demand=[]
  edge_name=[]
  edge_color=[]
  edge_width=[]
  edge_hi_width=[]
  edge_font_size=[]
  edge_length=[]
  edge_cost=[]
  edge_min_cap=[]
  edge_max_cap=[]
  edge_q_weight=[]
  edge_q_orig=[]
  edge_weight=[]
  default_node_diam=15
  default_node_border=1
  default_edge_width=1
  default_edge_hi_width=4
  default_font_size=1
  node_label=[]
  edge_label=[]
  
  GraphList=tlist(tt,name,directed,nodenumber,tail,head,..
	node_name,node_type,node_x,..
	node_y,node_color,node_diam,node_border,..
	node_font_size,node_demand,edge_name,..
	edge_color,edge_width,edge_hi_width,edge_font_size,..
	edge_length,edge_cost,..
	edge_min_cap,edge_max_cap,edge_q_weight,..
	edge_q_orig,edge_weight,..
	default_node_diam,default_node_border,default_edge_width,..
	default_edge_hi_width,..
	default_font_size,node_label,edge_label);
endfunction
