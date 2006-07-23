function [g1]=contract_edge(i,j,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>3 then error(39), end
//contracting the edges between node i and node j
g1=delete_arcs([i j],g);
g1=delete_arcs([j i],g1);
ta=g1('tail');he=g1('head');
ii=find(ta==j);ta(ii)=i*ones(ii);
ii=find(he==j);he(ii)=i*ones(ii);
g1('tail')=ta;g1('head')=he;
xnodes=g('node_x');ynodes=g('node_y');
if xnodes<>[] then
  x1=xnodes(i);x2=xnodes(j);
  y1=ynodes(i);y2=ynodes(j);
else
  x1=[],x2=[],y1=[],y2=[]
end
if (x1<>[]) then 
  xnodes(i)=0.5*(x1+x2);
  g1('node_x')=xnodes;
end;
if (y1<>[]) then
  ynodes(i)=0.5*(y1+y2);
  g1('node_y')=ynodes;
end;
g1=delete_nodes(j,g1);
endfunction
