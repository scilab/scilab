
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [cir]=hamilton(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // compute lp, la and ln
  m=prod(size(g.edges.tail));
  n=node_number(g);
  if(g.directed<>1) then 
    error('The graph must be directed')
  end
  [lp,la,ln]=adj_lists(1,n,g.edges.tail,g.edges.head);
  pr=lp-1;np1=n+1;
  //
  nc=1;nb=-1;
  //nc = number of circuits to be found (nc=-1 means all)
  //nb = number of backtracking to be performed (nb=-1 means exact procedure)
  [s]=m6hamil(n,m,np1,pr,ln,nc,nb); s=[s s(1)];
  if(s <> []) then cir=nodes_2_path(s,g);else cir=[];end;
endfunction
