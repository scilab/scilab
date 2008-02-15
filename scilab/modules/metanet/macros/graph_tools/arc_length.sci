
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function len=arc_length(g)
// returns the length of the arcs of the graph g
  ma=edge_number(g)
  if or(getfield(1,g.edges.data)=='length')& g.edges.data.length<>[] then 
    //there is a a data field for arc lengths
    mmi=min(g.edges.data.length); mma=max(g.edges.data.length);
    if (mmi<=0|mma<=0) then
      len=ones(1,ma)
    else
      len=g.edges.data.length;
    end
  else
    len=ones(1,ma)
  end
  if ~g.directed then
    len=[len len]
  end
endfunction
