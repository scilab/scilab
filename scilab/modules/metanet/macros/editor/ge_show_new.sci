
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_show_new(GraphList,scale)
  if argn(2)<2 then scale=1,end
  w=string(win)
  execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
  if edited then
    if messagebox(['Current graph is modified'
		  'Do you really want to erase it'],"modal","question",['yes','no'])==2 then
      return
    end
  end
  execstr('EGdata_'+w+'.Zoom=scale')
  execstr('EGdata_'+w+'.GraphList=GraphList')
  execstr('EGdata_'+w+'.Edited=%f;EGdata=EGdata_'+w)
  ge_clear_history()
  ge_do_replot(GraphList)
endfunction
