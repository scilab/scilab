
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [in,out]=ge_get_nodes_in_rect(GraphList,rect)
  in=[];out=[]
  x=GraphList.nodes.graphics.x;
  y=GraphList.nodes.graphics.y;
  ox=rect(1);oy=rect(2),w=rect(3),h=rect(4)
  in=matrix(find( ((ox-x).*(ox+w-x))<0 & ((oy-y).*(oy-h-y))<0),1,-1)
  out=1:size(GraphList.nodes.graphics.x,'*'),out(in)=[];
endfunction
