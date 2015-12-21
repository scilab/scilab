// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//=============================================================================
function res = dlwMake(files, objects_or_dll)

    if ~haveacompiler() then
        error(msprintf(gettext("%s: A Fortran or C compiler is required.\n"), "G_make"))
    end

    [lhs,rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "G_make"));
    end

    msg = "";

    if typeof(objects_or_dll)<>"string" then
        error(sprintf(gettext("%s: Wrong type for input argument #%d: String expected."), "G_make", 2));
    end

    OBJ = "";
    [ptmp, ftmp, fext] = fileparts(objects_or_dll);
    OBJ = ptmp + ftmp;

    //create a scibuild.bat file in TMPDIR directory
    cmd = "nmake /Y /nologo /f Makefile.mak " + OBJ;
    scibuildfile = writeBatchFile(cmd);
    if ilib_verbose() > 1 then
        msg = unix_g(scibuildfile);
        disp(msg);
    else
        host(scibuildfile);
    end

    deletefile(scibuildfile);
    res = [OBJ];

endfunction
//=============================================================================
function filename = writeBatchFile(cmd)

    //update DEBUG_SCILAB_DYNAMIC_LINK to match with Scilab compilation mode
    val = getenv("DEBUG_SCILAB_DYNAMIC_LINK","");
    debugVal = "NO";
    if val <> "YES" & val <> "NO" & isDebug() then
        debugVal = "YES";
    end

    if win64() then
        arch = "x64";
    else
        arch = "x86";
    end

    path = dlwGetVisualStudioPath();

    scibuild = [ ...
    "@call """ + path + "\VC\vcvarsall.bat"" " + arch;
    "set DEBUG_SCILAB_DYNAMIC_LINK=" + debugVal;
    cmd
    ];

    filename = TMPDIR + "/scibuild.bat";
    mputl(scibuild, filename);
    //filename = "call " + filename;
endfunction
