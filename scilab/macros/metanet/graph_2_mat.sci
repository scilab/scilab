function [a]=graph_2_mat(g,mat)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  mat='node-arc'
else
  if rhs<>2 then error(39), end 
end

// g
check_graph(g)

n=g('node_number')
ma=edge_number(g)
m=arc_number(g);
ta=g('tail'); he=g('head');
// node-arc matrix
if mat=='node-arc' then
 if g('directed') == 1 then
   u=sparse([ta' [1:ma]'],ones(ma,1),[n;ma]);
   v=sparse([he' [1:ma]'],-1*ones(ma,1),[n;ma]);
 else
   u=sparse([ta' [1:ma]'],ones(ma,1),[n;ma]);
   v=sparse([he' [1:ma]'],ones(ma,1),[n;ma]);
 end
 a=u+v
elseif mat=='node-node'
// node-node matrix
 if (g('directed')==1) then
   ij=[ta' he'];
   a=sparse(ij,ones(m,1),[n n]);
 else
   ij=[[ta he]' [he ta]'];
   a=sparse(ij,ones(m,1),[n n]);
 end
else
 error('Second argument must be ""node-node"" or ""node-arc""')
end
endfunction
