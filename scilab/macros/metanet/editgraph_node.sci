function node=editgraph_node(v1,v2,v3,v4,v5)
//Copyright INRIA
//Author : Serge Steer 2002

//Block data structure initialization
  if exists('orig','local')==0 then orig=[0,0],end
  if exists('name','local')==0 then name='Untitled',end

   tt = ['node','node_name','node_type','node_x',..
	'node_y','node_color','node_diam','node_border',..
	'node_font_size','node_demand','node_label']  
  
  node_name=name;
  node_type=0;
  node_x=orig(1);
  node_y=orig(2);
  node_color=1;
  node_diam=GraphList.default_node_diam;
  node_border=GraphList.default_node_border;
  node_font_size=GraphList.default_font_size;
  node_demand=0;
  node_label='';
  
  node=tlist(tt,node_name,node_type,node_x,..
	node_y,node_color,node_diam,node_border,..
	node_font_size,node_demand,node_label);
endfunction

