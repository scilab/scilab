// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function datatipSetDisplay(curve_handle,fun)
//Changes the datatips visualisation function for a given curve
  ud=datatipGetStruct(curve_handle)
  if typeof(ud)<>'datatips' then;
    if ~datatipInitStruct(curve_handle) then return,end
    ud=datatipGetStruct(curve_handle)
  end
  if argn(2)==1 then
    // function definition is queried
    txt=fun2string(ud.formatfunction,'fun')
    while %t then
      txt=x_dialog('Function Editor',txt)
      if txt==[] then ok=%f,break,end
      ierr=execstr(txt,'errcatch')
      if ierr==0 then ok=%t,break,end
      messagebox(lasterror(),'','error')
    end
    if ~ok then return,end
  end
  if ~datatipCheckFunction(fun) then return,end
  ud.formatfunction=fun
  datatipSetStruct(curve_handle,ud)
  for k=1:size(ud.tips,'*')
    tip_handle=ud.tips(k)
    point_handle=tip_handle.children(1)
    string_handle=tip_handle.children(2)
    tip_index=point_handle.user_data(2)
    string_handle.text=fun(curve_handle,point_handle.data)
  end

endfunction
function ok=datatipCheckFunction(fun)
  ok=%f
  [out,in,c]=string(fun)
  if size(in,'*')<>3 then
    messagebox(_("The selected function must have 3 input parameters (curve,pt,index)"))
    return
  end
  if size(out,'*')<>1 then
    messagebox(_("The selected function must have 1 output parameter (str)"))
    return
  end
  ok=%t
endfunction
