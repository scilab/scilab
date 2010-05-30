// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwHaveCompiler()
  bOK = (findmsvccompiler() <> 'unknown');
  if win64() then
    if bOK & detectmsvc64tools() then
      bOK = %T;
    else
      bOK = %F;
    end
  end
endfunction
//=============================================================================
