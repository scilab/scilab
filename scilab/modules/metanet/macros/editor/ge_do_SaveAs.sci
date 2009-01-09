
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [GraphList,ok,fname]=ge_do_SaveAs(GraphList,fname)
  tit='Select a file path'
  fname=stripblanks(fname)
  if fname=='' then
    path=fname
  else
    [path,name,ext]=splitfilepath(fname)
  end
  if stripblanks(path)=='' then path='./',end
  fname=tk_savefile('*.graph',path,tit)
  
  fname=stripblanks(fname)
  if fname==emptystr() then ok=%f,return,end

  [path,name,ext]=splitfilepath(fname)
  
//following test already done by tk_savefile
//   if fileinfo(fname)<>[] then
//     r=messagebox(['File '+fname+ ' already exists'
// 		 'erase it'],"modal","question",['Yes','No'])
//     if r==2 then ok=%f,return,end
//   end
  ok=execstr('save(fname,GraphList)','errcatch')==0
  if ok then 
    GraphList.name=name
  else
    messagebox([lasterror();'';'Graph has not been saved'],"modal","error");
  end    
endfunction
