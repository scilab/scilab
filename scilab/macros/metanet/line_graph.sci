function [g1]=line_graph(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
tta=g('tail');hhe=g('head');
ta=[tta hhe];he=[hhe tta];
ma=prod(size(g('tail')));
ta1=[];he1=[];
for i=1:(2*ma),
  ist=ta(i);iar=he(i);
  ij=find(ta==ist);ij=ij-int((ij-0.1)/ma)*ma;ij=ij(find(ij>i));
  jk=find(he==iar);jk=jk-int((jk-0.1)/ma)*ma;jk=jk(find(jk>i));
  ke=[ij jk];ka=i*ones(ke);
  ta1=[ta1 ka];he1=[he1 ke];
end;
g1=make_graph('foo',0,ma,ta1,he1);
xi=g('node_x');yi=g('node_y');
if (xi<>[]) then
  x1i=0.5*(xi(tta)+xi(hhe));
  g1('node_x')=x1i;
end
if (yi<>[]) then
  y1i=0.5*(yi(tta)+yi(hhe));
  g1('node_y')=y1i;
end
endfunction
