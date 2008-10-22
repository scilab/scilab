// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path=get_function_path(name)
libname=whereis(name)
if libname==[] then
  warning(" """+name +'"" is not a library function')
  path=[]
else
  execstr('t=string('+libname+')')
  path=t(1)+name+'.sci'
  if strindex(path,['SCI/','SCI\'])==1 then
    path=SCI+part(path,4:length(path))
  end
  if fileinfo(path)==[] then
    warning(msprintf(gettext("%s: There is no file named %s.sci in the library directory %s.\n"),"get_function_path",name,t(1)))
    path=[]
  end
end
endfunction

