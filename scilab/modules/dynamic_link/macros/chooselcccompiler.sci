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
// force to use LCC-Win32 compiler
//==========================================
function res = chooselcccompiler()

  res = findlcccompiler();
  if (res) then
    global LCC;
    LCC=res;
    clear LCC;
    configure_lcc();
  end
  
endfunction
//==========================================
