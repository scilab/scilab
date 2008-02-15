
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function node=editgraph_node(v1,v2,v3,v4,v5)

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

