function [g1]=graph_simp(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
check_graph(g)
n=g('node_number');
ta=g('tail');he=g('head');
[ir ic]=find(ta==he);
ta(ic)=[];he(ic)=[];
p=round(log(n)/log(10));q=10^(p+4);
w=ta'*q+he';[w1 iw]=sort(w);
ww=w1(2:$)-w1(1:($-1));
[ir ic]=find(ww==0);
index=iw(ir);
ta(index)=[];he(index)=[];
spg=sparse([ta' he'],ones(size(ta,2),1),[n n]);
spg=spg+spg';
[ir ic]=find(spg==2);kk=[ir' ic'];
spg1=[];
if (kk <> []) then
  spg1=sparse(kk,ones(1,(size(ir,2))),[n n]);
end;
spg=spg-spg1;
spg=tril(spg,-1);
[ij,v,mn]=spget(spg);
g1=make_graph('foo',0,n,ij(:,1)',ij(:,2)');
g1('node_type')=g('node_type');
g1('node_x')=g('node_x');
g1('node_y')=g('node_y');
g1('node_color')=g('node_color');
g1('node_diam')=g('node_diam');
g1('node_border')=g('node_border');
g1('node_font_size')=g('node_font_size');
g1('node_demand')=g('node_demand');
endfunction
