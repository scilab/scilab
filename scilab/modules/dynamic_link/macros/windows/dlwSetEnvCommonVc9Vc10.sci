// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvCommonVc9Vc10(MSVSDir, IsExpress, bWin64)
    bOK = %F;

    windowsSdkPath = dlwGetSdkPath();
    if (windowsSdkPath <> []) then
        err = setenv("WindowsSdkDir", windowsSdkPath);
    else
        windowsSdkPath = "";
    end

    err = setenv("VSINSTALLDIR", MSVSDir);
    if (err == %F) then
        bOK = %F;
        return
    end

    DevEnvDir = MSVSDir + "\Common7\IDE";
    err = setenv("DevEnvDir", DevEnvDir);
    if (err == %F) then
        bOK = %F;
        return
    end

    err = setenv("VCINSTALLDIR", MSVSDir + "\VC\");
    if (err == %F) then
        bOK = %F;
        return
    end

    if bWin64 then
        VC_PATH = getVcPathx64(MSVSDir, windowsSdkPath, IsExpress);
        VC_INCLUDE = getVcIncludex64(MSVSDir, windowsSdkPath, IsExpress);
        VC_LIB = getVcLibx64(MSVSDir, windowsSdkPath, IsExpress);
        VC_LIBPATH = getVcLibPathx64(MSVSDir, windowsSdkPath, IsExpress);
    else
        VC_PATH = getVcPathx86(MSVSDir, windowsSdkPath, IsExpress);
        VC_INCLUDE = getVcIncludex86(MSVSDir, windowsSdkPath, IsExpress);
        VC_LIB = getVcLibx86(MSVSDir, windowsSdkPath, IsExpress);
        VC_LIBPATH = getVcLibPathx86(MSVSDir, windowsSdkPath, IsExpress);
    end

    err = addPathToEnv("PATH", VC_PATH);
    if (err == %F) then
        bOK = %F;
        return
    end

    err = addPathToEnv("INCLUDE", VC_INCLUDE);
    if (err == %F) then
        bOK = %F;
        return
    end

    err = addPathToEnv("LIB", VC_LIB);
    if (err == %F) then
        bOK = %F;
        return
    end

    err = addPathToEnv("LIBPATH", VC_LIBPATH);
    if (err == %F) then
        bOK = %F;
        return
    end

    bOK = %T;
endfunction
//=============================================================================
function VcLib = getVcLibx64(msvsPath, sdkPath, bIsExpress)
    if bIsExpress then
        VcLib = dlwGet64BitPath() + "\VC\lib\amd64";
    else
        VcLib = msvsPath + "\VC\atlmfc\lib\amd64" + pathsep() + ..
        msvsPath + "\VC\lib\amd64";
    end

    VcLib = VcLib + pathsep() + sdkPath + "\lib\x64";
endfunction
//=============================================================================
function VcLib = getVcLibx86(msvsPath, sdkPath, bIsExpress)
    VcLib = "";
    if ~bIsExpress then
        VcLib =  msvsPath + filesep() + "VC\atlmfc\lib";
    end
    VcLib = VcLib + pathsep() + ..
    msvsPath + "\VC\lib" + pathsep() + ..
    sdkPath + "\lib";
endfunction
//=============================================================================
function VcPath = getVcPathx64(msvsPath, sdkPath, bIsExpress)
    if bIsExpress then
        VcPath = dlwGet64BitPath();
    else
        VcPath = msvsPath;
    end

    VcPath = VcPath + "\VC\bin\amd64" + pathsep() + ..
    msvsPath + "\VC\VCPackages" + pathsep() + ..
    msvsPath + "\Common7\IDE" + pathsep() + ..
    msvsPath + "\Common7\Tools" + pathsep() + ..
    msvsPath + "\Common7\Tools\bin" + pathsep() + ..
    sdkPath + "\bin\x64" + pathsep() + ..
    sdkPath + "\bin\win64\x64" + pathsep() + ..
    sdkPath + "\bin";
endfunction
//=============================================================================
function VcPath = getVcPathx86(msvsPath, sdkPath, bIsExpress)
    VcPath = msvsPath + "\Common7\IDE\" + pathsep() + ..
    msvsPath + "\VC\bin" + pathsep() + ..
    msvsPath + "\Common7\Tools" + pathsep() + ..
    msvsPath + "\VC\VCPackages" + pathsep() + ..
    sdkPath + "\bin";
endfunction
//=============================================================================
function VcLibPath = getVcLibPathx64(msvsPath, sdkPath, bIsExpress)
    if bIsExpress then
        VcLibPath = dlwGet64BitPath() +  "\VC\lib\amd64";
    else
        VcLibPath = msvsPath + "\VC\atlmfc\lib\amd64" + pathsep() + ..
        msvsPath + "\VC\lib\amd64";
    end
endfunction
//=============================================================================
function VcLibPath = getVcLibPathx86(msvsPath, sdkPath, bIsExpress)
    VcLibPath = "";
    if ~bIsExpress then
        VcLibPath = msvsPath + "\VC\atlmfc\lib";
    end

    VcLibPath = VcLibPath + pathsep() + ..
    msvsPath + "\VC\lib";
endfunction
//=============================================================================
function VcInclude = getVcIncludex64(msvsPath, sdkPath, bIsExpress)
    if bIsExpress then
        VcInclude = dlwGet64BitPath() + "\VC\include";
    else
        VcInclude = msvsPath + "\VC\include" + pathsep() + ..
        msvsPath + "\VC\atlmfc\include";
    end

    VcInclude = VcInclude + pathsep() + ..
    sdkPath + "\include";
endfunction
//=============================================================================
function VcInclude = getVcIncludex86(msvsPath, sdkPath, bIsExpress)
    VcInclude = "";
    if ~bIsExpress then
        VcInclude = msvsPath + "\VC\atlmfc\include";
    end

    VcInclude = VcInclude + pathsep() + ..
    msvsPath + "\VC\include"  + pathsep() + ..
    sdkPath + "\include";
endfunction
//=============================================================================
