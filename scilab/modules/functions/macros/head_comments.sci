
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function head_comments(name,%paths)
//displays the first comments of a function
  if exists('%paths')==0 then %paths='./',end
  name=stripblanks(name)
  if exists(name)==0 then 
    error(msprintf(gettext("%s: Undefined function %s.\n"),"head_comments",name)),
  end
  execstr('t=type('+name+')')
  if t<>11 & t<>13 then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: A Scilab function name is expected.\n"),"head_comments",1))
  end
  l=whereis(name)
  if l<>[] then
    execstr('path=string('+l+');path=path(1)')
    path=path+name+'.sci'
  else
    files= listfiles(%paths+'*.sci')
    if files==[] then
      error(msprintf(gettext("%s: ''%s'' file cannot be found in the given paths.\n"),"head_comments",name))
    end
    k=grep(files,name+'.sci')
    if k<>[] then
      path=files(k(1))
    else
      path=[]
    end
  end
  if path==[] then
    error(msprintf(gettext("%s: ''%s'' file cannot be found in the given paths.\n"),"head_comments",name))
  end
  txt=mgetl(path);
  k=grep(txt,'function');
  if k==[] then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: A Scilab function name is expected.\n"),"head_comments",1))

  end
  head=txt(k(1))
  txt=txt(k(1)+1:$)
  K=grep(part(txt,1:2),'//')
  if K(1)<>1 then 
     mprintf(gettext("No comment available."))
     return
  end
  k2=find(K(2:$)-K(1:$-1)<>1,1)
  if k2==[] then k2=size(K,'*'),end
  sel=K(1):k2(1)
  if sel<>[] then
    mprintf("%s\n",[head;strsubst(txt(sel),'//','')])
  else
    mprintf(gettext("No comment available."))
  end
endfunction
