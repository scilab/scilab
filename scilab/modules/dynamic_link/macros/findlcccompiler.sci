//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2007
// Detect LCC-Win32 compiler
//-----------------------------------------------------------------------------
function res=findlcccompiler()
  if MSDOS then
    res=%F;
    try
      winqueryreg('HKEY_CURRENT_USER','Software\lcc\compiler','includepath')
      res=%T;
    catch
      res=%F;
    end
    if (res) then
      try
        winqueryreg('HKEY_CURRENT_USER','Software\lcc\lcclnk','libpath')
        res=%T;
      catch
        res=%F;
      end
    end
  else
    res=%F
  end
endfunction
//-----------------------------------------------------------------------------

