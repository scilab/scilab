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
function r = with_lcc()

 if MSDOS then // Windows
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      r = %F;
    else
      // LCC-WIN32
      if ( findlcccompiler() == %T ) then
        r = %T;
      else
      // another compiler
        r = %F;
      end
    end
  
  else // LINUX
    r = %F;
  end

endfunction
//==========================================