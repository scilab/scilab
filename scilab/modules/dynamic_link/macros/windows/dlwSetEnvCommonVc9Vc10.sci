// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwSetEnvCommonVc9Vc10(MSVSDir, IsExpress, bWin64)
  bOK = %F;

  windowsSdkPath = dlwGetSdkPath();
  if (windowsSdkPath <> []) then
    err = setenv('WindowsSdkDir', windowsSdkPath);
  else
    windowsSdkPath = '';
  end

  err = setenv('VSINSTALLDIR', MSVSDir);
  if (err == %F) then
    bOK = %F;
    return
  end

  DevEnvDir = MSVSDir + '\Common7\IDE';
  err = setenv('DevEnvDir', DevEnvDir);
  if (err == %F) then
    bOK = %F;
    return
  end

  err = setenv('VCINSTALLDIR', MSVSDir + '\VC\');
  if (err == %F) then
    bOK = %F;
    return
  end

  PATH = getenv('PATH', '');
  INCLUDE = getenv('INCLUDE', '');
  LIB = getenv('LIB', '');
  LIBPATH = getenv('LIBPATH', '');

  if bWin64 then
    PATH = getNewPATHx64(PATH, MSVSDir, windowsSdkPath, IsExpress);
    INCLUDE = getNewINCLUDEx64(INCLUDE, MSVSDir, windowsSdkPath, IsExpress);
    LIB = getNewLIBx64(LIB, MSVSDir, windowsSdkPath, IsExpress);
    LIBPATH = getNewLIBPATHx64(LIBPATH, MSVSDir, windowsSdkPath, IsExpress);
  else
    PATH = getNewPATHx86(PATH, MSVSDir, windowsSdkPath, IsExpress);
    INCLUDE = getNewINCLUDEx86(INCLUDE, MSVSDir, windowsSdkPath, IsExpress);
    LIB = getNewLIBx86(LIB, MSVSDir, windowsSdkPath, IsExpress);
    LIBPATH = getNewLIBPATHx86(LIBPATH, MSVSDir, windowsSdkPath, IsExpress);
  end

  err = setenv('PATH', PATH);
  if (err == %F) then
    bOK = %F;
    return
  end

  err = setenv('INCLUDE', INCLUDE);
  if (err == %F) then
    bOK = %F;
    return
  end

  err = setenv('LIB', LIB);
  if (err == %F) then
    bOK = %F;
    return
  end

  err = setenv('LIBPATH', LIBPATH);
  if (err == %F) then
    bOK = %F;
    return
  end

  bOK = %T;
endfunction
//=============================================================================
function newLIB = getNewLIBx64(LIB, msvsPath, sdkPath, bIsExpress)
  newLIB = '';

  if bIsExpress then
    Vc64Path = dlwGet64BitPath();
    newLIB = Vc64Path + '\VC\LIB\amd64' + pathsep();
  else
    newLIB = msvsPath + '\VC\ATLMFC\LIB\amd64' + pathsep() + ..
             msvsPath + '\VC\LIB\amd64' + pathsep();
  end

  newLIB = newLIB + ..
           sdkPath + '\lib\x64' + pathsep() + LIB;

endfunction
//=============================================================================
function newLIB = getNewLIBx86(LIB, msvsPath, sdkPath, bIsExpress)
  newLIB = '';
  if ~bIsExpress then
    newLIB =  msvsPath + filesep() + 'VC\ATLMFC\LIB' + pathsep();
  end
  newLIB = newLIB + ..
           msvsPath + filesep() + 'VC\LIB' + pathsep() + ..
           sdkPath + filesep() + 'lib' + pathsep() + ..
           LIB;
endfunction
//=============================================================================
function newPATH = getNewPATHx64(PATH, msvsPath, sdkPath, bIsExpress)
  newPATH = '';

  if bIsExpress then
    Vc64Path = dlwGet64BitPath();
    newPATH = Vc64Path + '\VC\BIN\amd64' + pathsep();
  else
    newPATH = msvsPath + '\VC\BIN\amd64' + pathsep();
  end

  newPATH = newPATH + ..
            msvsPath + '\VC\VCPackages' + pathsep() + ..
            msvsPath + '\Common7\IDE' + pathsep() + ..
            msvsPath + '\Common7\Tools' + pathsep() + ..
            msvsPath + '\Common7\Tools\bin' + pathsep() + ..
            sdkPath + '\bin\x64' + pathsep() + ..
            sdkPath + '\bin\win64\x64' + pathsep() + ..
            sdkPath + '\bin' + pathsep() + PATH;

endfunction
//=============================================================================
function newPATH = getNewPATHx86(PATH, msvsPath, sdkPath, bIsExpress)
  newPATH = msvsPath + '\Common7\IDE\' + pathsep() + ..
            msvsPath + '\VC\bin' + pathsep() + ..
            msvsPath + '\Common7\Tools'+ pathsep() + ..
            msvsPath + '\VC\VCPackages' + pathsep() + ..
            sdkPath + '\bin' + pathsep() + PATH + pathsep();
endfunction
//=============================================================================
function newLIBPATH = getNewLIBPATHx64(LIBPATH, msvsPath, sdkPath, bIsExpress)
  newLIBPATH = '';

  if bIsExpress then
    Vc64Path = dlwGet64BitPath();
    newLIBPATH = Vc64Path +  '\VC\LIB\amd64' + pathsep() + LIBPATH;
  else
    newLIBPATH = msvsPath + '\VC\ATLMFC\LIB\amd64' + pathsep() + ..
                 msvsPath + '\VC\LIB\amd64' + pathsep() + LIBPATH;
  end

endfunction
//=============================================================================
function newLIBPATH = getNewLIBPATHx86(LIBPATH, msvsPath, sdkPath, bIsExpress)
  newLIBPATH = '';

  if ~bIsExpress then
    newLIBPATH = msvsPath + '\VC\ATLMFC\LIB' + pathsep();
  end

  newLIBPATH = newLIBPATH + ..
               msvsPath + '\VC\LIB';
endfunction
//=============================================================================
function newINCLUDE = getNewINCLUDEx64(INCLUDE, msvsPath, sdkPath, bIsExpress)
  newINCLUDE = '';
  if bIsExpress then
    Vc64Path = dlwGet64BitPath();
    newINCLUDE = Vc64Path + '\VC\INCLUDE' + pathsep();
  else
    newINCLUDE = msvsPath + '\VC\INCLUDE' + pathsep() + ..
                 msvsPath + '\VC\ATLMFC\INCLUDE' + pathsep();
  end

  newINCLUDE = newINCLUDE + ..
               sdkPath + '\include' + pathsep() + INCLUDE;

endfunction
//=============================================================================
function newINCLUDE = getNewINCLUDEx86(INCLUDE, msvsPath, sdkPath, bIsExpress)
  newINCLUDE = '';

  if ~bIsExpress then
    newINCLUDE = msvsPath + '\VC\ATLMFC\INCLUDE'  + pathsep();
  end

  newINCLUDE = newINCLUDE + ..
               msvsPath + '\VC\INCLUDE'  + pathsep() + ..
               sdkPath + '\include' + pathsep() + INCLUDE;
endfunction
//=============================================================================
