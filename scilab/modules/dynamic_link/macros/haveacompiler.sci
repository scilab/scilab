// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
function bOK = haveacompiler()
  if MSDOS then
    bOK = %F;
    msvc = findmsvccompiler();
    if ( msvc == 'unknown' ) then
      if ~win64() & findlcccompiler() then
        bOK = %T;
      else
        bOK = %F;
      end
    else
      if win64() then
        if detectmsvc64tools() then
          bOK = %T;
        else
          bOK = %F;
        end
      else
        bOK = %T;
      end
    end
  else
    // Very hard to detect under Linux/Unix since there are plenty of 
	  // compiler... Then, we return all the time TRUE
	  // The actual detection is done by the dedicated ./configure
    bOK = %T;
  end
endfunction
//==========================================
