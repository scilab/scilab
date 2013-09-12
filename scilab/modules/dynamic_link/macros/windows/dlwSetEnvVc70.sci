// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvVc70()
    bOK = %F;

    MSVCDir = dlwGetVc70Path();
    if (MSVCDir == []) then
        return
    end

    err = setenv("MSVCDir", MSVCDir);
    if (err == %F) then
        bOK = %F;
        return
    end

    err = setenv("DevEnvDir", MSVCDir + filesep() + "..\Common7\Tools");
    if (err == %F) then
        bOK = %F;
        return
    end

    DevEnvDir = getenv("DevEnvDir", "ndef");
    if (DevEnvDir == "ndef") then
        bOK = %F;
        return
    end

    PATH = getenv("PATH", "ndef");
    if (PATH == "ndef") then
        bOK = %F;
        return
    end

    err = addPathToEnv("PATH", MSVCDir + "\bin" + pathsep() + ..
    DevEnvDir + pathsep() + ..
    DevEnvDir + "\bin" + pathsep() + ..
    MSVCDir + "\..\Common7\IDE");
    if (err == %F) then
        bOK = %F;
        return
    end

    err = addPathToEnv("include", MSVCDir + "\atlmfc\include" + pathsep() + ..
    MSVCDir + "\include" + pathsep() +..
    MSVCDir + "\PlatformSDK\include");
    if (err == %F) then
        bOK = %F;
        return
    end

    err = setenv("lib", MSVCDir + "\atlmfc\lib" + pathsep() + ..
    MSVCDir + "\lib"+ pathsep() +..
    MSVCDir + "\PlatformSDK\lib");
    if (err == %F) then
        bOK = %F;
        return
    end

    bOK = %T;

endfunction
//=============================================================================
