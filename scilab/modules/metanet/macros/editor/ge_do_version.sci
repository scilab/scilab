
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_version(GraphList)

  for f=ge_node_fields()
    GraphList(f)=matrix(GraphList(f),1,-1)
  end
  for f=ge_arc_fields()
    GraphList(f)=matrix(GraphList(f),1,-1)
  end
  
endfunction
