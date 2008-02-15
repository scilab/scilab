
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [k,wh]=ge_getobj(GraphList,pt)

  wh=[],
  k=ge_getnode(pt)
  if k<>[] then wh='node',return,end
  k=ge_getedge(GraphList,pt)
  if k<>[] then wh='arc',return,end

endfunction
