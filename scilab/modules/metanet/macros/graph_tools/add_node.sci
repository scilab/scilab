function [g1]=add_node(g,xy,name)
// Copyright INRIA
[lhs,rhs]=argn(0)
select rhs
case 1 then
  xy=[]
  name=0
case 2 then
  if type(xy)==1 then name=0
  else name=xy, xy=[], end
else
  if rhs<>3 then error(39), end
end
// check xy
if xy<>[] then
  s=size(xy);
  if (s(1)<>1|s(2)<>2) then
    error('""xy"" must be a row vector with size 2')
  end
end
// check name
if name<>0 then
  if type(name)<>10|prod(size(name))<>1 then
    error('""name"" must be a string')
  end
end
// adding a node at the (x,y) position with default values
g1=g;n=g('node_number');
g1('node_number')=n+1;
if (g('node_name')<>[]) then
  if name==0 then 
    g1('node_name')=[g1('node_name') string(n+1)];
  else 
    g1('node_name')=[g1('node_name') name];
  end
else
  if name<>0 then 
    g1('node_name')=[string(1:n) name]
  end
end;
if (g('node_type')<>[]) then g1('node_type')=[g1('node_type') 0];end;
if g('node_x')<>[]&g('node_y')<>[] then
  if xy<>[] then
    g1('node_x')=[g1('node_x') xy(1)];
    g1('node_y')=[g1('node_y') xy(2)];
  else
    g1('node_x')=[g1('node_x') 0];
    g1('node_y')=[g1('node_y') 0];
  end
end
if (g('node_color')<>[]) then g1('node_color')=[g1('node_color') 0];end;
if (g('node_diam')<>[]) then 
  g1('node_diam')=[g1('node_diam') g1('default_node_diam')];
end;
if (g('node_border')<>[]) then 
  g1('node_border')=[g1('node_border') g1('default_node_border')];
end;
if (g('node_font_size')<>[]) then 
  g1('node_font_size')=[g1('node_font_size') g1('default_font_size')];
end;
if (g('node_demand')<>[]) then g1('node_demand')=[g1('node_demand') 0];end;
if (g1('node_label')<>[]) then
  g1('node_label')=[g1('node_label') string(n+1)];
end;
endfunction
