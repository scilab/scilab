function g1=graph_simp(g)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  check_graph(g)
  n=node_number(g);
  ta=g.edges.tail;
  he=g.edges.head;
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
  g1=make_graph('Untitled',0,n,ij(:,1)',ij(:,2)');
  g1.nodes=g.nodes
endfunction
