
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function D=%s_i_ngraphic(i,d,D)
  FD=getfield(1,D);
  if d<>[] then error('Invalid assignment'),end
  FD(FD=='defaults'|FD=='display'|FD=='displaymode')=[];
  for f=FD(2:$)
    D(f)(:,i)=[]
  end
endfunction
