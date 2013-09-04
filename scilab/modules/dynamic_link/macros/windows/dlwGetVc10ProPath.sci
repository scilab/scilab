// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function vcPath = dlwGetVc10ProPath()
    vcPath = [];
    try
        vcPath = winqueryreg("HKEY_LOCAL_MACHINE", ..
        "Software\Microsoft\VisualStudio\10.0\Setup\VS", ..
        "ProductDir");
    catch
        // remove last error on 'winqueryreg' fails
        lasterror();
        return;
    end

    // remove last file separator if it exists
    if vcPath <> [] then
        vcPath = pathconvert(vcPath, %f, %t);
    end

endfunction
//=============================================================================
