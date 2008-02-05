//==========================================
// INRIA 2008
// Allan CORNET
// Sylvestre LEDRU
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
    // Very hard to detect under Linux/Unix since there are plenty of 
	// compiler... Then, we return all the time TRUE
	// The actual detection is done by the dedicated ./configure
    bOK = %T;
  end
endfunction
//==========================================
