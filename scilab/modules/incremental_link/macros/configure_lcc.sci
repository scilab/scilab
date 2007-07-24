//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2007
//-----------------------------------------------------------------------------
function bOK=configure_lcc()
  if MSDOS then
    try
      lccincludepath=winqueryreg('HKEY_CURRENT_USER','Software\lcc\compiler','includepath');
      lcclibpath=winqueryreg('HKEY_CURRENT_USER','Software\lcc\lcclnk','libpath');
      index=strindex(lccincludepath,filesep());
      szindex=size(index);
      lccbasepath=part(lccincludepath,[1:index(szindex(2))]);
      lccbinpath=lccbasepath+'bin';
      lccexe=lccbinpath+'\lcc.exe';
      if (fileinfo(lccexe)<>[]) then
        PATH=getenv('PATH');
        NEWPATH=lccbinpath+';'+lccincludepath+';'+lcclibpath+';'+PATH;
        setenv('PATH',NEWPATH);
        bOK=%T;
      else
        bOK=%F;
      end
    catch
      bOK=%F;
    end
  else  
    bOK=%F;
  end
endfunction
//-----------------------------------------------------------------------------
