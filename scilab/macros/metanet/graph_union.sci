function [g2]=graph_union(g,g1)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
//check g and g1
check_graph(g);
check_graph(g1);
// union of 2 graphs
n=g('node_number'); 
n1=g1('node_number');
ta=g('tail');he=g('head');
ta1=g1('tail');he1=g1('head');
no=g('node_name');no1=g1('node_name');
ic1=[];index=[];index1=[];
for i=1:n1,
  [ir,ic]=find(no==no1(i));
  if (ic<>[]) then index=[index i]; end;
  ic1=[ic1 ic];
end
ninter=size(ic1,2);
ng2=n+n1-ninter;
uu=[1:n1];uu(index)=[];
w=[1:(size(uu,2))];
ww=w+n;
w=[1:n1];w(uu)=ww;
w(index)=ic1;
ta11=w(ta1);he11=w(he1);
//
ta2=[ta ta11];he2=[he he11];
g2=make_graph('foo',g('directed'),ng2,ta2,he2);
//
a=g('node_name');a1=g1('node_name');
if((a<>[])&(a1<>[])) then 
  a1(index)=[];g2('node_name')=[a a1];
end;
a=g('node_type');a1=g1('node_type');
if((a<>[])&(a1<>[])) then 
  a1(index)=[];g2('node_type')=[a a1];
end;
a=g('node_x');a1=g1('node_x');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_x')=[a a1];
end;
a=g('node_y');a1=g1('node_y');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_y')=[a a1];
end;
a=g('node_color');a1=g1('node_color');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_color')=[a a1];
end;
a=g('node_diam');a1=g1('node_diam');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_diam')=[a a1];
end;
a=g('node_border');a1=g1('node_border');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_border')=[a a1];
end;
a=g('node_font_size');a1=g1('node_font_size');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_font_size')=[a a1];
end;
a=g('node_demand');a1=g1('node_demand');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_demand')=[a a1];
end;
a=g('node_label');a1=g1('node_label');
if((a<>[])&(a1<>[])) then
  a1(index)=[];g2('node_label')=[a a1];
end;
//
a=g('edge_name');a1=g1('edge_name');
if((a<>[])&(a1<>[])) then
  g2('edge_name')=[a a1];
end;
a=g('edge_color');a1=g1('edge_color');
if((a<>[])&(a1<>[])) then
  g2('edge_color')=[a a1];
end;
a=g('edge_width');a1=g1('edge_width');
if((a<>[])&(a1<>[])) then
  g2('edge_width')=[a a1];
end;
a=g('edge_hi_width');a1=g1('edge_hi_width');
if((a<>[])&(a1<>[])) then
  g2('edge_hi_width')=[a a1];
end;
a=g('edge_font_size');a1=g1('edge_font_size');
if((a<>[])&(a1<>[])) then
  g2('edge_font_size')=[a a1];
end;
a=g('edge_length');a1=g1('edge_length');
if((a<>[])&(a1<>[])) then
  g2('edge_length')=[a a1];
end;
a=g('edge_cost');a1=g1('edge_cost');
if((a<>[])&(a1<>[])) then
  g2('edge_cost')=[a a1];
end;
a=g('edge_min_cap');a1=g1('edge_min_cap');
if((a<>[])&(a1<>[])) then
  g2('edge_min_cap')=[a a1];
end;
a=g('edge_max_cap');a1=g1('edge_max_cap');
if((a<>[])&(a1<>[])) then
  g2('edge_max_cap')=[a a1];
end;
a=g('edge_q_weight');a1=g1('edge_q_weight');
if((a<>[])&(a1<>[])) then
  g2('edge_q_weight')=[a a1];
end;
a=g('edge_q_orig');a1=g1('edge_q_orig');
if((a<>[])&(a1<>[])) then
  g2('edge_q_orig')=[a a1];
end;
a=g('edge_weight');a1=g1('edge_weight');
if((a<>[])&(a1<>[])) then
  g2('edge_weight')=[a a1];
end;
a=g('edge_label');a1=g1('edge_label');
if((a<>[])&(a1<>[])) then
  g2('edge_label')=[a a1];
end;
endfunction
