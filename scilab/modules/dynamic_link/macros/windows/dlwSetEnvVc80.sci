// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvVc80(msCompiler)
    bOK = %F;
    MSVSDir = "";
    select msCompiler
    case "msvc80pro"
        MSVSDir = dlwGetVc80ProPath();
    case "msvc80std"
        MSVSDir = dlwGetVc80StdPath();
    case "msvc80express"
        MSVSDir = dlwGetVc80ExpressPath();
    else
        return
    end

    // MS compiler path is wrong
    if MSVSDir == [] then
        return
    end

    PATH = getenv("PATH","ndef");
    if (PATH == "ndef") then
        bOK = %F;
        return
    end

    if ~setenv("VSINSTALLDIR", MSVSDir) then
        bOK = %F;
        return
    end

    MSVCDir = MSVSDir + "\VC";
    if ~setenv("VCINSTALLDIR", MSVCDir) then
        bOK = %F;
        return
    end

    DevEnvDir = MSVSDir + "\Common7\IDE";
    if ~setenv("DevEnvDir", DevEnvDir) then
        bOK = %F;
        return
    end

    err = addPathToEnv("PATH", DevEnvDir + pathsep() + ..
    MSVCDir + "\bin" + pathsep() + ..
    MSVSDir + "\Common7\Tools" + pathsep() + ..
    MSVSDir + "\SDK\v2.0\bin" + pathsep() + ..
    MSVCDir + "\VCPackages");
    if (err == %F) then
        bOK = %F;
        return
    end

    if (msCompiler == "msvc80express") then
        windowsSdkPath = dlwGetSdkPath();
        LIB = MSVCDir + "\lib" + pathsep() + ..
        MSVSDir + "\SDK\v2.0\lib" + pathsep() + ..
        windowsSdkPath + "\lib";

        include = MSVCDir + "\include" + pathsep() + ..
        windowsSdkPath + "\include";
    else
        LIB = MSVCDir + "\lib" + pathsep() + ..
        MSVSDir + "\SDK\v2.0\lib" + pathsep() + ..
        MSVSDir + "\VC\PlatformSDK\lib";

        include = MSVCDir + "\include" + pathsep() + ..
        MSVCDir + "\PlatformSDK\include" + pathsep() + ..
        MSVSDir + "\SDK\v2.0\include";
    end

    if ~addPathToEnv("LIB", LIB) then
        bOK = %F;
        return
    end

    if ~addPathToEnv("include", include) then
        bOK = %F;
        return
    end

    bOK = %T;
endfunction
//=============================================================================
