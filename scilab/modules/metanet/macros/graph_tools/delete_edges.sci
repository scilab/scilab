
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=delete_edges(ij,g)
  if argn(2)<>2 then error(39), end

  // checking ij
  if (ij==[]) then return,; end;
  if (type(ij)<>1|size(ij,2) <> 2) then
    error('First argument must be a n x 2 scalar vector')
  end;
  n=node_number(g);
  if or(ij<1|ij>n) then
    error('Node numbers should be in [1 '+string(n)+']')
  end
  
  //search edges index connecting the given node pairs
  ta=g.edges.tail;he=g.edges.head;
  ic=[];
  if(g.directed == 1) then
    for jj=1:size(ij,1),
      ic=[ic find((ta==ij(jj,1)&(he==ij(jj,2))))]
    end
  else
    for jj=1:size(ij,1),
      i=ij(jj,1);j=ij(jj,2);
      ic=[ic find((ta==i)&(he==j))];
      ic=[ic find((ta==j)&(he==i))];    
    end;
  end;
  
  // deleting the edges
  g.edges(ic)=[]
endfunction
