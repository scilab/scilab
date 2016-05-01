// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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
