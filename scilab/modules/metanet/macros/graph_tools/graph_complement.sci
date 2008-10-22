
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=graph_complement(g,gmax)
  [lhs,rhs]=argn(0);
  if rhs==1 then
    ind=0;
  elseif rhs==2 then
    check_graph(gmax);
    ind=1;
  else
    error(39)
  end
  check_graph(g);
  n=node_number(g);
  m=edge_number(g)
  ta=g.edges.tail;he=g.edges.head;
  spg=sparse([ta' he'],ones(m,1),[n n]);
  if ind==1 then
    // gmax given
    nma=node_number(gmax);mma=prod(size(gmax.edges.tail));
    if (nma <> n) then
      error('The two graphs must have the same number of nodes')
    end
    ama=gmax.edges.tail;bma=gmax.edges.head;
    spgma=sparse([ama' bma'],ones(mma,1),[nma nma]);
    spgma=spgma+spgma';
    spdif=spgma-spg-spg';
    [ir ic]=find(spdif=-1);
    if((ir <> [])|(ic <> [])) then
      error('First graph must be a subgraph of second graph')
    end
    spdif=tril(spdif,-1);
    [ij,v,mn]=spget(spdif);
  else
    // gmax not given
    taa=[1:n].*.ones(n,1);hee=taa';
    taa=taa(:);hee=hee(:);
    ij=[taa hee];v=ones(n*n,1);
    spcomp=sparse(ij,v,[n n]);
    spdif=spcomp-spg-spg';
    spdif=tril(spdif,-1);
    [ij,v,mn]=spget(spdif);
  end
  //
  
  g1=make_graph('Untitled',0,n,ij(:,1)',ij(:,2)');
  g1.nodes=g.nodes
endfunction
