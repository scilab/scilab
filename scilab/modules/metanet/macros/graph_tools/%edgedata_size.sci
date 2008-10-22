
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s=%edgedata_size(d)
  Fd=getfield(1,d)
  if size(Fd,'*')<2 then 
    s=0
  else
    s=size(d(Fd(2)));s=s(2)
  end
endfunction
