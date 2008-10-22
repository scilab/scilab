
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [node,radius]=graph_center(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  check_graph(g,%f)

  // compute lp, la and ls
  ma=edge_number(g);
  n=node_number(g)
  head=g.edges.head;
  len=arc_length(g)
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,head,n+1,n)
    // compute fe, che, fn, chn
    [fe,che,fn,chn]=chain_struct(lp,la,ln)
  else
    tail=[g.edges.tail head];head=[head g.edges.tail]; 
    [lp,la,ln]=m6ta2lpd(tail,head,n+1,n);
    // compute fe, che, fn, chn
    [fe,che,fn,chn]=chain_struct(lp,la,ln)
  end
  // compute center
  [node,radius]=m6cent(fe,che,head,len,n+1)
endfunction
