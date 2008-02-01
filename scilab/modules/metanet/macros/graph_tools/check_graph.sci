function check_graph(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
if type(g)<>16 then
  error('Graph list check: the graph must be a typed list')
end
l=34
if size(g)<>l then
  error('Graph list check: the graph must be a list of size '+string(l))
end
// type
if g(1)(1)<>'graph' then 
  error('Graph list check: the list is not a graph')
end
// name
s=size(g(2))
if type(g(2))<>10|prod(s)<>1 then
  error('Graph list check: ""name"" must be a string')
end
// directed
if g(3)<>1&g(3)<>0 then
  error('Graph list check: ""directed"" must be 0 or 1')
end
// node_number
if prod(size(g(4)))<>1|g(4)<1
  error('Graph list check: ""node_number"" must be greater than 1')
end
n=g(4)
// tail
s=size(g(5))
if s(1)<>1 then
  error('Graph list check: ""tail"" must be a row vector')
end
ma=s(2)
// head
s=size(g(6))
if s(1)<>1 then
  error('Graph list check: ""head"" must be a row vector')
end
if s(2)<>ma then
  error('Graph list check: ""tail"" and ""head"" must have identical sizes')
end
// tail and head
if min(min(g(5)),min(g(6)))<1|max(max(g(5)),max(g(6)))>n then
  error('Graph list check: ""tail"" and ""head"" do not represent a graph')
end
// node_name
s=size(g(7))
if prod(s)<>0 then
  if type(g(7))<>10 then
    error('Graph list check: ""node_name"" must be a string row vector')
  end
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_name"" must be a row vector of size '+string(n))
  end
end
// node_type
s=size(g(8))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_type"" must be a row vector of size '+string(n))
  end  
end
// node_x
s=size(g(9))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_x"" must be a row vector of size '+string(n))
  end  
end
// node_y 
s=size(g(10))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_y"" must be a row vector of size '+string(n))
  end  
end
// node_color
s=size(g(11))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_color"" must be a row vector of size '+string(n))
  end  
end
// node_diam
s=size(g(12))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_diam"" must be a row vector of size '+string(n))
  end  
end
// node_border
s=size(g(13))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_border"" must be a row vector of size '+string(n))
  end  
end
// node_font_size
s=size(g(14))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_font_size"" must be a row vector of size '+string(n))
  end  
end
// node_demand
s=size(g(15))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_demand"" must be a row vector of size '+string(n))
  end  
end
// edge_name
s=size(g(16))
if prod(s)<>0 then
  if type(g(16))<>10 then
    error('Graph list check: ""edge_name"" must be a string row vector')
  end
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_name"" must be a row vector of size '+string(ma))
  end
end
// edge_color
s=size(g(17))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_color"" must be a row vector of size '+string(ma))
  end  
end
// edge_width
s=size(g(18))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_width"" must be a row vector of size '+string(ma))
  end  
end
// edge_hi_width
s=size(g(19))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_hi_width"" must be a row vector of size '+string(ma))
  end  
end
// edge_font_size
s=size(g(20))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_font_size"" must be a row vector of size '+string(ma))
  end  
end
// edge_length
s=size(g(21))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_length"" must be a row vector of size '+string(ma))
  end  
end
// edge_cost
s=size(g(22))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_cost"" must be a row vector of size '+string(ma))
  end  
end
// edge_min_cap
s=size(g(23))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_min_cap"" must be a row vector of size '+string(ma))
  end  
end
// edge_max_cap
s=size(g(24))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_max_cap"" must be a row vector of size '+string(ma))
  end  
end
// edge_q_weight
s=size(g(25))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_q_weight"" must be a row vector of size '+string(ma))
  end  
end
// edge_q_orig
s=size(g(26))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_q_orig"" must be a row vector of size '+string(ma))
  end  
end
// edge_weight
s=size(g(27))
if prod(s)<>0 then
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_weight"" must be a row vector of size '+string(ma))
  end  
end
// default_node_diam
s=size(g(28))
if prod(s)<>0 then
  if prod(s)<>1 then
    error('Graph list check: ""default_node_diam"" must be a scalar')
  end  
end
// default_node_border
s=size(g(29))
if prod(s)<>0 then
  if prod(s)<>1 then
    error('Graph list check: ""default_node_border"" must be a scalar')
  end  
end
// default_edge_width
s=size(g(30))
if prod(s)<>0 then
  if prod(s)<>1 then
    error('Graph list check: ""default_edge_width"" must be a scalar')
  end  
end
// default_edge_hi_width
s=size(g(31))
if prod(s)<>0 then
  if prod(s)<>1 then
    error('Graph list check: ""default_edge_hi_width"" must be a scalar')
  end  
end
// default_font_size
s=size(g(32))
if prod(s)<>0 then
  if prod(s)<>1 then
    error('Graph list check: ""default_font_size"" must be a scalar')
  end  
end
// node_label
s=size(g(33))
if prod(s)<>0 then
  if type(g(33))<>10 then
    error('Graph list check: ""node_label"" must be a string row vector')
  end
  if s(1)<>1|s(2)<>n then
    error('Graph list check: ""node_label"" must be a row vector of size '+string(n))
  end
end
// edge_label
s=size(g(34))
if prod(s)<>0 then
  if type(g(34))<>10 then
    error('Graph list check: ""edge_label"" must be a string row vector')
  end
  if s(1)<>1|s(2)<>ma then
    error('Graph list check: ""edge_label"" must be a row vector of size '+string(ma))
  end
end
endfunction
