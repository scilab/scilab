function [outdegree,indegree]=nodes_degrees(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
ta=g('tail');he=g('head');n=g('node_number');np1=n+1;
i=g('directed')
[lp,la,ln] = adj_lists(i,n,ta,he);
outdegree=lp(2:np1)-lp(1:n);
[lp,la,ln] = adj_lists(i,n,he,ta);
indegree=lp(2:np1)-lp(1:n);
endfunction
