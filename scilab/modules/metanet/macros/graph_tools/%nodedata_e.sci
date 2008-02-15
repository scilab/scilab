
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%nodedata_e(i,d)
  F=getfield(1,d);F=F(2:$)
  if i==[] then
    for f=F,d(f)=[],end
  else
    for f=F,d(f)=d(f)(:,i),end
  end
endfunction
