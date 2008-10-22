//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Detect LCC-Win32 compiler
//==========================================
function res = findlcccompiler()
  res = %F;
  if MSDOS then
    try
      lccincludepath = winqueryreg('HKEY_CURRENT_USER', ..
                                   'Software\lcc\compiler', ..
                                   'includepath');
                        
    catch
      // remove last error on 'winqueryreg' fails
      lasterror();
      return;
    end
  
    try
      winqueryreg('HKEY_CURRENT_USER', ..
                  'Software\lcc\lcclnk', ..
                  'libpath');
      index = strindex(lccincludepath,filesep());
      szindex = size(index);
      lccbasepath = part(lccincludepath,[1:index(szindex(2))]);
      lccbinpath = lccbasepath + 'bin';
      lccexe = lccbinpath + filesep() + 'lcc.exe';
      if ( fileinfo(lccexe) <> [] ) then
        res = %T;
      end
    catch
      // remove last error on 'winqueryreg' fails
      lasterror();
      return; 
    end
  end
endfunction
//==========================================

