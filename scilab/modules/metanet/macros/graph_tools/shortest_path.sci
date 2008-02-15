
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [p,lp]=shortest_path(i,j,g,typ)
  [lhs,rhs]=argn(0)
  if rhs==3 then
    typ='arc'
  else
    if rhs<>4 then error(39), end
  end
  // check i and j
  if prod(size(i))<>1 then
    error('First argument must be a scalar')
  end
  if prod(size(j))<>1 then
    error('Second argument must be a scalar')
  end
  // check typ
  if type(typ)<>10|prod(size(typ))<>1 then
    error('Fourth argument must be a string')
  end
  // check g
  check_graph(g,%f)
  // compute lp, la and ln
  n=node_number(g); 
  ma=prod(size(g.edges.tail))
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end


  // compute shortest path according to typ
  if typ=='arc' then
    [l,v]=m6pcchna(i,lp,ln,n)
    p=m6prevn2p(i,j,v,la,lp,ln,g.directed)
    lp=l(j)
  else
    if and(getfield(1,g.edges.data)<>typ) then
      error('No '''+typ+''' data field is defined')
    end
    // value of length
    if g.edges.data(typ)==[] then
      len=zeros(1,ma)
    else
      len=g.edges.data(typ)
    end
    lneg=find(len<0)
    if lneg<>[] then 
      [l,v]=m6ford(i,la,len,lp,ln,n)
    else   
      if ma<0.5*n*n then 
	[l,v]=m6johns(i,la,len,lp,ln,n)
      else 
	[l,v]=m6dijkst(i,la,len,lp,ln,n)
      end
    end
    p=m6prevn2p(i,j,v,la,lp,ln,g.directed)
    lp=l(j)
  end
endfunction
