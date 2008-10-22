
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function D=%edges_i_edges(i,d,D)
  D.tail(i)=d.tail 
  D.head(i)=d.head
  D.data(i)=d.data
  D.graphics(i)=d.graphics
endfunction
