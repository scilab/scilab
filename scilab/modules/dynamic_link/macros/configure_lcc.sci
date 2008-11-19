//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//==========================================
function bOK = configure_lcc()
  bOK = %F;
  
  if MSDOS then

    try
      lccincludepath = winqueryreg('HKEY_CURRENT_USER','Software\lcc\compiler','includepath');
    catch
      return;
    end

    try
      lcclibpath = winqueryreg('HKEY_CURRENT_USER','Software\lcc\lcclnk','libpath');
    catch
      return;
    end
      
    index = strindex(lccincludepath,filesep());
    szindex = size(index);
    lccbasepath = part(lccincludepath,[1:index(szindex(2))]);
    lccbinpath = lccbasepath + 'bin';
    lccexe = lccbinpath + filesep() + 'lcc.exe';
      
    if ( fileinfo(lccexe) <> [] ) then
      PATH = getenv('PATH');
      NEWPATH = lccbinpath + pathsep() + ..
                lccincludepath + pathsep() + ..
                lcclibpath + pathsep() + ..
                PATH;
      err = setenv('PATH',NEWPATH);
      if (err == %F) then bOK = %F,return,end
      
      LCCLIB_DIR = SCIHOME+'\lcclib';
      
      if ( (fileinfo(LCCLIB_DIR) == []) | ( findfiles(LCCLIB_DIR,'*.lib') == []) ) then
        mprintf('\n');
        mprintf(gettext('%s: Converts libraries to use LCC-Win32.\n'),'configure_lcc')
        bOK =  VCtoLCCLib();
      else
        bOK = %T;
      end
      
    else
      bOK=%F;
    end
  end
  
endfunction
//==========================================
