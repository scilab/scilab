function [g1]=arc_graph(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
if g('directed')<>1 then
  error('The graph must be directed')
end
ma=prod(size(g('tail')));
ta=g('tail');he=g('head');
ta1=[];he1=[];
for i=1:ma,
  ist=ta(i);iar=he(i);
  ij=find(ta==iar);
  ka=i*ones(ij);
  ta1=[ta1 ka];he1=[he1 ij];
end;
if (ta1==[]|he1==[]) then
  g1=[]; return
end
g1=make_graph('foo',1,ma,ta1,he1);
xi=g('node_x');yi=g('node_y');
if (xi<>[]) then
  x1i=0.5*(xi(ta)+xi(he));
  g1('node_x')=x1i;
end
if (yi<>[]) then
  y1i=0.5*(yi(ta)+yi(he));
  g1('node_y')=y1i;
end
endfunction
