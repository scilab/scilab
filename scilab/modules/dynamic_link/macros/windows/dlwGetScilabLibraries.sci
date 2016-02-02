// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
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
function scilablibrarieslist = dlwGetScilabLibraries()

    scilablibrarieslist = ["blasplus.lib"; ..
    "libf2c.lib"; ..
    "core.lib"; ..
    "core_f.lib"; ..
    "lapack.lib"; ..
    "output_stream.lib"; ..
    "dynamic_link.lib"; ..
    "integer.lib"; ..
    "optimization_f.lib"; ..
    "libjvm.lib"; ..
    "scilocalization.lib"; ..
    "linpack_f.lib"; ..
    "call_scilab.lib"; ..
    "time.lib"; ..
    "api_scilab.lib"; ..
    "libintl.lib"; ..
    "ast.lib"; ..
    "fileio.lib"; ..
    "io.lib"; ..
    "string.lib"; ..
    "threads.lib"; ..
    "sciconsole.lib"; ..
    "scilab_windows.lib"; ..
    "libmex.lib"];
endfunction
//=============================================================================
