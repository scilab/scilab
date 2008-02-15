
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function l=netwindows()
//w=winsid()
  G=who('global')
  G=G(part(G,1:7)=='EGdata_')
  if G==[] then
    wins=[],cur=0
  else
    global EGcurrent
    wins=evstr(part(G,8:11))
    cur=EGcurrent
  end
  l=list(wins',cur)
endfunction

