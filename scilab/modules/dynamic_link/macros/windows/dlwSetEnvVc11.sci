// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvVc11(msCompiler, bWin64)
  bOK = %F;
  MSVSDir = '';
  select msCompiler
    case 'msvc110pro'
      MSVSDir = dlwGetVc11ProPath();
    case 'msvc110express'
      MSVSDir = dlwGetVc11ExpressPath();
  else
    return
  end
  
  // MS compiler path is wrong
  if MSVSDir == [] then
    return
  end

  IsExpress = (msCompiler == 'msvc110express');

  VS110COMNTOOLS = MSVSDir + '\Common7\Tools\'
  if ~setenv('VS110COMNTOOLS', VS110COMNTOOLS) then
    bOK = %F;
    return
  end

  bOK = dlwSetEnvCommonVc11(MSVSDir, IsExpress, bWin64);

endfunction
//=============================================================================
