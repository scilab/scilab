// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwSetEnvVc10(msCompiler, bWin64)
    bOK = %F;
    MSVSDir = "";
    select msCompiler
    case "msvc100pro"
        MSVSDir = dlwGetVc10ProPath();
    case "msvc100express"
        MSVSDir = dlwGetVc10ExpressPath();
    else
        return
    end

    // MS compiler path is wrong
    if MSVSDir == [] then
        return
    end

    IsExpress = (msCompiler == "msvc100express");

    VS100COMNTOOLS = MSVSDir + "\Common7\Tools\"
    if ~setenv("VS100COMNTOOLS", VS100COMNTOOLS) then
        bOK = %F;
        return
    end

    bOK = dlwSetEnvCommonVc9Vc10(MSVSDir, IsExpress, bWin64);

endfunction
//=============================================================================
