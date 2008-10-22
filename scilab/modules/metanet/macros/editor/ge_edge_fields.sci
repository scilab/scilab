
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [fa,fd]=ge_edge_fields()
  F=getfield(1,GraphList.edges)
  fa = F(2:$) //skip the type, the default field and the data field
  F=getfield(1,GraphList.edges.data)
  fd = F(2:$);
endfunction
