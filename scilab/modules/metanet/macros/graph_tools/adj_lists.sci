
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [lp,la,ln]=adj_lists(g,n,tail,head)
  [lhs,rhs]=argn(0)
  if rhs==1 then
    // g
    check_graph(g,%f)
    directed=g.directed
    n=node_number(g)
    ma=edge_number(g)
    tail=g.edges.tail
    head=g.edges.head
    
  else
    if rhs<>4 then error(39), end
    directed=g
    // check directed
    if directed<>1&directed<>0 then
      error('First argument must be 0 or 1')
    end
    // check n
    if size(n,'*')<>1|n<1
      error('Second argument must be greater than 1')
    end
    if size(tail,'*')<>size(head,'*') then
      error('""tail"" and ""head"" must have identical sizes')
    end
 
    // check tail
    ma=size(tail,'*')
    tail=matrix(tail,1,-1)
    head=matrix(head,1,-1)
    // check tail and head
    if min(min(tail),min(head))<1|max(max(tail),max(head))>n then
      error('""tail"" and ""head"" do not represent a graph')
    end
  end
  // compute lp, la and ln
  if directed==1 then
    [lp,la,ln]=m6ta2lpd(tail,head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(tail,head,n+1,n,2*ma)
  end
endfunction
