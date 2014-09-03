// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvVc12(msCompiler, bWin64)
  bOK = %F;
  MSVSDir = '';
  select msCompiler
    case 'msvc120pro'
      MSVSDir = dlwGetVc12ProPath();
    case 'msvc120express'
      MSVSDir = dlwGetVc12ExpressPath();
  else
    return
  end

  // MS compiler path is wrong
  if MSVSDir == [] then
    return
  end

  IsExpress = (msCompiler == 'msvc120express');

  VS110COMNTOOLS = MSVSDir + '\Common7\Tools\'
  if ~setenv('VS120COMNTOOLS', VS110COMNTOOLS) then
    bOK = %F;
    return
  end

  bOK = dlwSetEnvCommonVc12(MSVSDir, IsExpress, bWin64);

endfunction
//=============================================================================
