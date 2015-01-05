// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//=============================================================================
function dlwGenerateCleaner(fd, makename)
    val = getenv("SCILAB_PREFERED_MSVC", "");
    make_command = dlwGetMakefileCmdCleaner(makename);
    mfprintf(fd,"if fileinfo(''%s'') <> [] then\n", makename);
    if val <> "" then
        mfprintf(fd,"  setenv(""SCILAB_PREFERED_MSVC"", ""%s"");\n", val);
    end
    mfprintf(fd,"  if ~ exists(""dynamic_linkwindowslib"") then\n");
    mfprintf(fd,"    load(""SCI/modules/dynamic_link/macros/windows/lib"")\n");
    mfprintf(fd,"  end\n");
    mfprintf(fd,"  dlwConfigureEnv();\n");
    mfprintf(fd,"  unix_s(''%s'');\n", make_command);
    mfprintf(fd,"  mdelete(''%s'');\n", makename);
    mfprintf(fd,"end\n");
    mfprintf(fd,"// ------------------------------------------------------\n");
endfunction
//=============================================================================
