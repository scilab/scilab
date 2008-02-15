
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=add_node(g,xy,name)
  
  //Check inputs
  //------------
  check_graph(g,%f)
  n=node_number(g);
  select argn(2)
  case 1 then //add a single node
    xy=[0 0]
    name=string(n+1)
  case 2 then
    if type(xy)==10 then 
      name=xy;
      xy=zeros(2,size(name,'*'))
    elseif type(xy)==1 then  
      if and(size(xy)==[1 2]) then xy=xy';end//single node
      name=string(n+(1:size(xy,2)))
    end
  case 3 then
    if type(xy)==10 then [xy,name]=(name,xy),end
    if type(xy)<>1 then error(52),end
    if and(size(xy)==[1 2]) then xy=xy';end//single node
    if size(xy,2)<>size(name,'*') then
      error('Node coordinates matrix and vector of names must have the same length')
    end
  end
  n1=size(xy,2)
  // adding a node at the (x,y) position with default values
  g.nodes.number=g.nodes.number+n1;
  if or(getfield(1, g.nodes)=='graphics') then
    g.nodes.graphics.name=[g.nodes.graphics.name name];
    if (g.nodes.graphics.type<>[]) then 
      g.nodes.graphics.type=[g.nodes.graphics.type zeros(1,n1)];
    end
    if g.nodes.graphics.x<>[]&g.nodes.graphics.y<>[] then
      g.nodes.graphics.x=[g.nodes.graphics.x xy(1,:)];
      g.nodes.graphics.y=[g.nodes.graphics.y xy(2,:)];
    end
    if g.nodes.graphics.colors<>[] then 
      g.nodes.graphics.colors=[g.nodes.graphics.colors zeros(2,n1)];
    end;
    if g.nodes.graphics.diam<>[]  then 
      g.nodes.graphics.diam  =[g.nodes.graphics.diam zeros(1,n1)];
    end;
    if g.nodes.graphics.border<>[] then 
      g.nodes.graphics.border=[g.nodes.graphics.border  zeros(1,n1)];
    end
    if g.nodes.graphics.font<>[] then 
      g.nodes.graphics.font  =[g.nodes.graphics.font zeros(3,n1)];
    end
    if n>0& or(getfield(1, g.nodes)=='data') then
      for k=1:n1
	g.nodes.data($+1)=g.nodes.data($)
      end
    end
  end
endfunction
