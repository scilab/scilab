
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  ok=ge_do_quit(check_if_edited)
  if argn(2)<1 then check_if_edited=%t,end
  global EGcurrent
  w=string(win)
  execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
  if edited&check_if_edited then
    ok=messagebox(['Current graph is modified'
		  'Really quit?'],"modal","question",['yes','no'])
    if ok==2 then ok=%f,return,end
  end
  if or(win==winsid()) then seteventhandler(''),end
  if win==EGcurrent then EGcurrent=[],end
  execstr('clearglobal EGdata_'+w+' EGhist_'+w)
  ok=%t
endfunction
