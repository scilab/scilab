
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  netwindow(win)

  global EGcurrent
  w=string(win)
  execstr('global  EGdata_'+w+',ok=typeof(EGdata_'+w+')==''egdata''')
  if ~ok then
    execstr('clearglobal  EGdata_'+w)
    error('Bad edit_graph window number: '+string(win))
  end
  EGcurrent=win 
endfunction

