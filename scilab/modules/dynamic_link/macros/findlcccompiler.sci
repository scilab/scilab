//==========================================
// Allan CORNET
// INRIA 2008
// Detect LCC-Win32 compiler
//==========================================
function res = findlcccompiler()
  res = %F;
  if MSDOS then
    try
      winqueryreg('HKEY_CURRENT_USER', ..
                  'Software\lcc\compiler', ..
                  'includepath');
    catch
      return;
    end
  
    try
      winqueryreg('HKEY_CURRENT_USER', ..
                  'Software\lcc\lcclnk', ..
                  'libpath');
      res = %T;
    catch
      return; 
    end
  end
endfunction
//==========================================

