// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwSetEnvVc80(msCompiler)
  bOK = %F;
  MSVSDir = '';
  select msCompiler
    case 'msvc80pro'
      MSVSDir = dlwGetVc80ProPath();
    case 'msvc80std'
      MSVSDir = dlwGetVc80StdPath();
    case 'msvc80express'
      MSVSDir = dlwGetVc80ExpressPath();
  else
    return
  end

  PATH = getenv('PATH','ndef');
  if (PATH == 'ndef') then
    bOK = %F;
    return
  end

  if ~setenv('VSINSTALLDIR', MSVSDir) then
    bOK = %F;
    return
  end

  MSVCDir = MSVSDir + filesep() + 'VC';
  if ~setenv('VCINSTALLDIR', MSVCDir) then
    bOK = %F;
    return
  end

  DevEnvDir = MSVSDir + filesep() +'Common7\IDE';
  if ~setenv('DevEnvDir', DevEnvDir) then
    bOK = %F;
    return
  end

  err = setenv('PATH', DevEnvDir + pathsep() + ..
               MSVCDir + filesep() + 'bin' + pathsep() + ..
               MSVSDir + filesep() + 'Common7\Tools' + pathsep() + ..
               MSVSDir + filesep() + 'SDK\v2.0\bin' + pathsep() + ..
               MSVCDir + filesep() + 'VCPackages' + pathsep() + ..
               PATH + pathsep() );
  if (err == %F) then
    bOK = %F;
    return
  end

  LIB = getenv('LIB', '');
  INCLUDE = getenv('INCLUDE', '');

  if (msCompiler == 'msvc80express') then
    windowsSdkPath = dlwGetSdkPath();
    LIB = MSVCDir + filesep() + 'LIB' + pathsep() + ..
        MSVSDir + filesep() + 'SDK\v2.0\lib' + pathsep() + ..
        windowsSdkPath + filesep() + 'Lib' + pathsep() + ..
        LIB;

    INCLUDE = MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
              windowsSdkPath + 'INCLUDE' + pathsep();
  else
    LIB = MSVCDir + filesep() + 'LIB' + pathsep() + ..
        MSVSDir + filesep() + 'SDK\v2.0\lib' + pathsep() + ..
        MSVSDir + filesep() + 'VC\PlatformSDK\lib' + pathsep() + ..
        LIB;

    INCLUDE = MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
              MSVCDir + filesep() + 'PlatformSDK\include' + pathsep() + ..
              MSVSDir + filesep() + 'SDK\v2.0\include' + pathsep() + ..
              INCLUDE;
  end

  if ~setenv('LIB', LIB) then
    bOK = %F;
    return
  end

  if ~setenv('INCLUDE', INCLUDE) then
    bOK = %F;
    return
  end

  bOK = %T;
endfunction
//=============================================================================
