
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=subgraph(v,ind,g)
  if argn(2)<>3 then error(39), end
  // check v
  s=size(v)
  if and(size(v)<>1) then
    error('First argument must be a vector')
  end
  // check ind
  if and(ind<>['nodes' 'edges']) then
    error('Second argument must be ""nodes"" or ""edges""')
  end
  // check g
  check_graph(g)
  // subgraph given by edges or nodes

  if ind=='edges' then // case of edges, v is the index of the edges to keep
    kept_arcs=unique(v)
    ne=edge_number(g);
    if kept_arcs(1)<1|kept_arcs($)>ne then
      error('Edge numbers should be in [1 '+string(ne)+']')
    end
     //find nodes to keep
    kept_nodes=unique([g.edges.tail(kept_arcs) g.edges.head(kept_arcs)]);
  else // case of nodes, v is the index of the nodes to keep
    kept_nodes=unique(v)
    nn=node_number(g);
    if kept_nodes(1)<1|kept_nodes($)>nn then
      error('Node numbers should be in [1 '+string(nn)+']')
    end
    kept_arcs=find(dsearch(g.edges.tail, kept_nodes,'d')>0&dsearch(g.edges.head, kept_nodes,'d')>0)
  end
  g.nodes=g.nodes(kept_nodes)
  g.edges=g.edges(kept_arcs)
  //renumber kept edges tail and head
  if kept_arcs<>[] then
    g.edges.tail=dsearch(g.edges.tail,kept_nodes,'d')
    g.edges.head=dsearch(g.edges.head,kept_nodes,'d')
  end
endfunction
