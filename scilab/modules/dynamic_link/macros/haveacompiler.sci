//==========================================
// INRIA 2008
// Allan CORNET
//==========================================
function bOK = haveacompiler()
  if MSDOS then
    bOK = %F;
    msvc = findmsvccompiler();
    if ( msvc == 'unknow' ) then
      if findlcccompiler() then
        bOK = %T;
      end
    else
      bOK = %T;
    end
  else
    // To do under Unix
    bOK = %T;
  end
endfunction
//==========================================
