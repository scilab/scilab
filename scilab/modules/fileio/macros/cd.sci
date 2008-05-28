// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path=cd(path)
  if argn(2)==1 then 
  	path=stripblanks(path)
    if or(path==['PWD' 'SCIHOME']) then path=evstr(path),end
    if MSDOS then 
      if or(path==['WSCI']) then path=evstr(path),end
    end
    if or(path==['SCI' '~' 'TMPDIR' 'home' ]) path=path+'/',end
    
    if or(path==['/' '\' '..']) then
      chdir(path)
    else
      if ~isdir(path) then
        error(msprintf(gettext("%s: Cannot go to directory %s\n"),'cd',path),998);
      end
      chdir(path)
    end 
  else 
    chdir(home);
  end
  path=pwd();
endfunction
