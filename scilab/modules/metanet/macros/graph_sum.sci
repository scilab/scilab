function [g2]=graph_sum(g,g1)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check g and g1
check_graph(g)
check_graph(g1)
n=g('node_number'); 
n1=g1('node_number');
if (n1 <> n) then
  error('The two graphs must have the same number of nodes')
end
// sum of 2 graphs (sum of adjacency matrices)
ta=g('tail');he=g('head');
ta1=g1('tail');he1=g1('head');
ta2=[ta ta1];he2=[he he1];ng2=size(ta2,2);
g2=make_graph('foo',g('directed'),n,ta2,he2);
//
g2('node_name')=g('node_name');
g2('node_type')=g('node_type');
g2('node_x')=g('node_x');
g2('node_y')=g('node_y');
g2('node_color')=g('node_color');
g2('node_diam')=g('node_diam');
g2('node_border')=g('node_border');
g2('node_font_size')=g('node_font_size');
g2('node_demand')=g('node_demand');
g2('node_label')=g('node_label');
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
