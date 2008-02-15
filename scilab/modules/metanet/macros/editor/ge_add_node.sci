
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_add_node(GraphList,node_x,node_y)
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
