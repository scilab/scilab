// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function p=tk_savefile(file_mask,path,Title,foo)
  warnobsolete("uigetfile", "5.2.1");
  if ~with_tk() then error(gettext("Tcl/Tk interface not loaded.")),end
  arg=''
  if exists('file_mask','local')==1 then 
    TCL_SetVar("ftypes",'{""Requested Extensions"" {'+file_mask+'} }')
    arg=arg+' -filetypes $ftypes',
  end
  if exists('path','local')==1 then arg=arg+' -initialdir ""'+path+'""',end
  if exists('Title','local')==1 then arg=arg+' -title ""'+Title+'""',end
  if exists('title','local')==1 then 
  Title=title
  arg=arg+' -title ""'+Title+'""'
  end
  if exists('ini','local')==1 then arg=arg+' -initialfile ""'+ini+'""',end
  TCL_EvalStr('set scifilepath [tk_getSaveFile'+arg+']')
  p=TCL_GetVar('scifilepath')
endfunction

