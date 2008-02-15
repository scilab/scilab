
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function edges=%edges_e(i,edges)
  edges.head=edges.head(i)
  edges.tail=edges.tail(i)
  edges.graphics=edges.graphics(i)
  edges.data=edges.data(i)
endfunction
