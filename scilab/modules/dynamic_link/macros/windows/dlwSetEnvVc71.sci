// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwSetEnvVc71()
  bOK = %F;
  MSVCDir = dlwGetVc71Path();
  if (MSVCDir == []) then
    return
  end

  err = setenv('MSVCDir', MSVCDir);
  if (err == %F) then
    bOK = %F;
    return
  end

  err = setenv('DevEnvDir', MSVCDir + filesep() + '..\Common7\Tools');
  if (err == %F) then
    bOK = %F;
    return
  end

  PATH = getenv('PATH', 'ndef');
  if (PATH =='ndef') then
    bOK = %F;
    return
  end

  DevEnvDir = getenv('DevEnvDir', 'ndef');
  if (DevEnvDir =='ndef') then
    bOK = %F;
    return
  end

  err = setenv('PATH',MSVCDir + filesep() + 'BIN' + pathsep() + ..
               DevEnvDir + pathsep() + ..
               DevEnvDir + filesep() + 'bin' + pathsep() + ..
               MSVCDir + filesep() + '..\Common7\IDE' + pathsep() + ..
               PATH + pathsep() );
  if (err == %F) then
    bOK = %F;
    return
  end

  INCLUDE = getenv('INCLUDE', '');

  err = setenv('INCLUDE', MSVCDir + filesep() + 'ATLMFC\INCLUDE' + pathsep() + ..
               MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
               MSVCDir + filesep() + 'PlatformSDK\include' + pathsep() + ..
               INCLUDE);
  if (err == %F) then
    bOK = %F;
    return
  end

  LIB = getenv('LIB', 'ndef');

  err = setenv('LIB', MSVCDir + filesep() + 'ATLMFC\LIB' + pathsep() + ..
               MSVCDir + filesep() + 'LIB' + pathsep() + ..
               MSVCDir + filesep() + 'PlatformSDK\lib' + pathsep() + ..
               LIB);
  if (err == %F) then
    bOK = %F;
    return
  end

  bOK = %T;

endfunction
//=============================================================================
