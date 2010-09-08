// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwSetEnvVc90(msCompiler, bWin64)
  bOK = %F;
  MSVSDir = '';
  select msCompiler
    case 'msvc90pro'
      MSVSDir = dlwGetVc90ProPath();
    case 'msvc90std'
      MSVSDir = dlwGetVc90StdPath();
    case 'msvc90express'
      MSVSDir = dlwGetVc90ExpressPath();
  else
    return
  end

  IsExpress = (msCompiler == 'msvc100express');

  if ~setenv('VS90COMNTOOLS', MSVSDir + '\Common7\Tools\') then
    bOK = %F;
    return
  end

  bOK = dlwSetEnvCommonVc9Vc10(MSVSDir, IsExpress, bWin64);

endfunction
//=============================================================================
