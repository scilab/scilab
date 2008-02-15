
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=graph_simp(g)
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
