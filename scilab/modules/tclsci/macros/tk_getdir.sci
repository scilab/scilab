// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function p=tk_getdir(startdir,Title)
  warnobsolete("uigetdir", "5.2.1");
  if ~with_tk() then error(gettext("Tcl/Tk interface not loaded.")),end
  arg=''
  if exists("startdir","local")==1 then 
    startdir=pathconvert(startdir,%f,%t)
    startdir=strsubst(startdir,"\","/")
    arg=arg+" -initialdir {"+startdir+"}"
  end
  if exists("title","local")==1 then
    Title=title
    arg = arg+" -title {"+Title+"}",
  elseif exists("Title","local")==1 then
    arg=arg+" -title {"+Title+"}";
  end
  arg=arg+" -parent $root"
  p=TCL_EvalStr("tk_chooseDirectory"+arg)
endfunction

