function g1=trans_closure(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g and connectivity (checking of g made in is_connex)
if is_connex(g)<>1 then
  error('The graph must be connected')
end
// compute lp, la and ln
n=g('node_number')
ma=prod(size(g('tail')))
if g('directed')==1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  error('The graph must be directed')
end
// compute lp1 and ln1 for the transitive closure
[lp1,ln1]=m6transc(lp,ln,n)
// compute tail1 and head1 (without checking) for the transitive closure
n1=prod(size(lp1))-1
m1=prod(size(ln1))
la1=1:m1
[tail1,head1]=m6lp2tad(lp1,la1,ln1,n1)
// compute transitive closure graph
g1=glist(g('name')+'_trans_closure',g('directed'),n1,tail1,head1,..
g('node_name'),g('node_type'),g('node_x'),g('node_y'),g('node_color'),..
g('node_diam'),g('node_border'),g('node_font_size'),g('node_demand'),..
[],[],[],[],[],[],[],[],[],[],[],[],..
g('default_node_diam'),g('default_node_border'),g('default_edge_width'),..
g('default_edge_hi_width'),g('default_font_size'),[],[])
endfunction
