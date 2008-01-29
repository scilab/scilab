//==========================================
// Allan CORNET
// INRIA 2008
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
      
      LCCLIB_DIR = SCI+'/bin/lcclib';
      
      if ( (fileinfo(LCCLIB_DIR) == []) | (ls(LCCLIB_DIR+'/*.lib') == []) ) then
        printf('\nConverts libraries to use LCC-Win32.\n');
        bOK = call_VCtoLCCLib();
      else
        bOK = %T;
      end
      
    else
      bOK=%F;
    end
  end
  
endfunction
//==========================================
function r = call_VCtoLCCLib()
  try
    VCtoLCCLib();
    r = %T;
  catch
    r = %F;
  end
endfunction
//==========================================
