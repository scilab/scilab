function g1=arc_graph(g)
//computes a new graph with nodes corresponding to arcs of the given one
// Copyright INRIA
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
