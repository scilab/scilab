function [g1]=supernode(v,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check v
s=size(v)
if s(1)<>1 then
  error('First argument must be a row vector')
end
// check g
check_graph(g)
//set of nodes v replaced by one
n=g('node_number'); 
vv=-sort(-v);
w=vv(2:$)-vv(1:($-1));
[ir,ic]=find(w==0);
vv(ic)=[];
if (vv(1)<1)|(vv($)>n) then
  error('A number in first argument is not a node number')
end
if vv($)==n then
   error('The graph must not be reduced to one node') 
end
g1=g;ne=size(g('tail'),2); 
a=g('tail');b=g('head');
a1=a;b1=b;
vta=[];vhe=[];
ndel=size(vv,2);
for i=1:ndel,
  ii=vv(i);
  [ir,ic]=find(a==ii); 
  vta=[vta ic];
  [ir,ic]=find(b==ii);
  vhe=[vhe ic];
end
if (size(vv,2)>1) then
  for i=size(vv,2):-1:2,
    ii=vv(i);
    [ir,ic]=find(a1>ii);
    if ic <> [] then 
      a1(ic)=a1(ic)-1;
    end;
    [ir,ic]=find(b1>ii);
    if ic <> [] then 
      b1(ic)=b1(ic)-1;
    end;
  end
end
a1(vta)=vv(1)*ones(vta);b1(vhe)=vv(1)*ones(vhe);
[ir,ic]=find((a1==vv(1))&(b1==vv(1)));
a1(ic)=[];b1(ic)=[];
noe=[1:ne];noe(ic)=[];
g1=make_graph('foo',g('directed'),(n+1-ndel),a1,b1);
idel=vv(2:$);ivv=vv(1);
a=g('node_type');a(idel)=[];g1('node_type')=a;
a=g('node_x');if a<>[] then a(ivv)=sum(a(vv))/ndel;end;a(idel)=[];g1('node_x')=a;
a=g('node_y');if a<>[] then a(ivv)=sum(a(vv))/ndel;end;a(idel)=[];g1('node_y')=a;
a=g('node_color');a(idel)=[];g1('node_color')=a;
//
if g('node_diam') <> [] then
  a=g('node_diam');nd1=[g('default_node_diam') a(ivv)];
  a(ivv)=2.*max(nd1);a(idel)=[];g1('node_diam')=a;
end;
//
if g('node_border') <> [] then
  a=g('node_border');nd1=[g('default_node_border') a(ivv)];
  a(ivv)=2.*max(nd1);a(idel)=[];g1('node_border')=a;
end;
//
a=g('node_font_size');a(idel)=[];g1('node_font_size')=a;
//
if g('node_demand') <> [] then
  a=g('node_demand');
  a(ivv)=sum(a(vv));a(idel)=[];g1('node_demand')=a;
end;
//
if g('node_label') <> [] then
  a=g('node_label');g1('node_label')=a(noe);
end;
//
a=g('edge_name');if a<>[] then g1('edge_name')=a(noe);end
a=g('edge_color');if a<>[] then g1('edge_color')=a(noe);end
a=g('edge_width');if a<>[] then g1('edge_width')=a(noe);end
a=g('edge_hi_width');if a<>[] then g1('edge_hi_width')=a(noe);end
a=g('edge_font_size');if a<>[] then g1('edge_font_size')=a(noe);end
a=g('edge_length');if a<>[] then g1('edge_length')=a(noe);end
a=g('edge_cost');if a<>[] then g1('edge_cost')=a(noe);end
a=g('edge_min_cap');if a<>[] then g1('edge_min_cap')=a(noe);end
a=g('edge_max_cap');if a<>[] then g1('edge_max_cap')=a(noe);end
a=g('edge_q_weight');if a<>[] then g1('edge_q_weight')=a(noe);end
a=g('edge_q_orig');if a<>[] then g1('edge_q_orig')=a(noe);end
a=g('edge_weight');if a<>[] then g1('edge_weight')=a(noe);end
if size(g('edge_label'),2) <> 0,
  a=g('edge_label');g1('edge_label')=a(noe);
end;
endfunction
