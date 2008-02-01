function [g1]=add_edge(i,j,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>3 then error(39), end
n=g('node_number')
ma=prod(size(g('tail')))
// check i and j
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
if (i<1|i>n) then
  error(string(i)+' is not a node number')
end
if prod(size(j))<>1 then
  error('Second argument must be a scalar')
end
if (j<1|j>n) then
  error(string(j)+' is not a node number')
end
// adding an edge from node i to node j
g1=g;
g1('tail')=[g('tail') i]; g1('head')=[g('head') j];
if g('edge_name')<>[] then g1('edge_name')=[g('edge_name') string(ma+1)];end;
if g('edge_color')<>[] then g1('edge_color')=[g('edge_color') 0];end;
if g('edge_width')<>[] then
  g1('edge_width')=[g('edge_width') g('default_edge_width')];
end;
if g('edge_hi_width')<>[] then
  g1('edge_hi_width')=[g('edge_hi_width') g('default_edge_hi_width')];
end;
if g('edge_font_size')<>[] then
  g1('edge_font_size')=[g('edge_font_size') g('default_font_size')];
end;
if g('edge_length')<>[] then g1('edge_length')=[g('edge_length') 0];end;
if g('edge_cost')<>[] then g1('edge_cost')=[g('edge_cost') 0];end;
if g('edge_min_cap')<>[] then g1('edge_min_cap')=[g('edge_min_cap') 0];end;
if g('edge_max_cap')<>[] then g1('edge_max_cap')=[g('edge_max_cap') 0];end;
if g('edge_q_weight')<>[] then g1('edge_q_weight')=[g('edge_q_weight') 0];end;
if g('edge_q_orig')<>[] then g1('edge_q_orig')=[g('edge_q_orig') 0];end;
if g('edge_weight')<>[] then g1('edge_weight')=[g('edge_weight') 0];end;
if g('edge_label')<>[] then
  g1('edge_label')=[g('edge_label') string(ma+1)];
end;
endfunction
