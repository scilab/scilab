
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [cir]=salesman(g,nstac)
  [lhs,rhs]=argn(0)
  n=node_number(g);
  if rhs>2|rhs<1 then 
    error(39)
  elseif rhs==1 then
    nstac=100*n*n
  end
  // check graph
  check_graph(g,%f);
  if(g.directed<>1) then 
    error('The graph must be directed')
  end
  check_graph(g);
  // check nstac
  if nstac<1 then
    error('The second argument must be positive')
  end
  m=arc_number(g);
  if and(edgedatafields(g)<>"length")|g.edges.data.length==[] then
    u=ones(1,m);
  else
    u=g.edges.data.length
  end
  ta=g.edges.tail;he=g.edges.head;
  uu=sparse([ta' he'],u',[n,n]);
  lsup=1000*n*max(u);
  uu1=sparse([ta' he'],lsup*ones(1,m)',[n,n]);
  a=full(uu);a=a+100000-uu1;
  s=m6visitor(n,a,nstac);s=[s s(1)]
  if(s <> []) then cir=nodes_2_path(s,g);else cir=[];end;
endfunction
