
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_delete_node(GraphList,K)

  if K==[] then return,end

  I=1:GraphList.nodes.number
  GraphList.nodes(K)=[]

  //renumber arcs head and tail

  for k=1:size(K,'*')
    I(K(k)+1:$)=I(K(k)+1:$)-1
  end
  GraphList.edges.head=I(GraphList.edges.head)
  GraphList.edges.tail=I(GraphList.edges.tail)
endfunction
