
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=arc_graph(g)
//computes a new graph with nodes corresponding to arcs of the given one
  if argn(2)<>1 then error(39), end
  // check g
  check_graph(g,%f)
  if g.directed<>1 then
    error('The graph must be directed')
  end
  ta=g.edges.tail;he=g.edges.head;
  ma=size(ta,'*');

  ta1=[];he1=[];
  for i=1:ma,
    ij=find(ta==he(i));
    ta1=[ta1 i*ones(ij)];
    he1=[he1 ij];
  end;
  if (ta1==[]|he1==[]) then
    g1=[]; return
  end
  g1=make_graph('foo',1,ma,ta1,he1);
  xi=g.nodes.graphics.x;
  yi=g.nodes.graphics.y;
  
  if (xi<>[]) then
    x1i=0.5*(xi(ta)+xi(he));
    g1.nodes.graphics.x=x1i;
  end
  if (yi<>[]) then
    y1i=0.5*(yi(ta)+yi(he));
    g1.nodes.graphics.y=y1i;
  end
endfunction
