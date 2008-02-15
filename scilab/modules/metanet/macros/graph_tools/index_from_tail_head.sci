
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function k=index_from_tail_head(g,t,h)
  
//find the index of the edges given by (tail head pairs)
  if size(h,'*')<>size(t,'*') then
    error('Second and third argument must have the same length')
  end

  n=node_number(g)
  if or(t<1|t>n|h<1|h>n) then
    error('Second and third argument does not represent edges of the given graph')
  end
  m=edge_number(g);
  sp=sparse([g.edges.tail' g.edges.head'],(1:m)',[n,n]);
  sp1=sparse([t(:) h(:)],ones(size(t,'*'),1),[n,n]);

  [ij,k,mn]=spget(sp.*sp1);
  multiple=find(k>m)
  if multiple<>[] then
    k(multiple)=[]
    for i=1:size(multiple,'*')
      k=[k;find(g.edges.tail==ij(i,1)&g.edges.head==ij(i,2))'];
    end
  end
  
  
  // find all the edges which have a tail index in t
//  [t,o]=gsort(t,'g','i');
//  i=dsearch(g.edges.tail,t,'d');
//  k=find(i<>0);i=i(i<>0)
  //k and i verify g.edges.tail(k)==t(i) so we search a subset of k
    
  // find the edges in k which have the correct head index
//  j=find(h(o(i))==g.edges.head(k))
//  k=k(j)
endfunction
