
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function D=%nodes_i_nodes(i,d,D)
  D.data(i)=d.data
  D.graphics(i)=d.graphics
  n=1:D.number;n(i)=1:d.number
  D.number=size(n,'*')
endfunction

