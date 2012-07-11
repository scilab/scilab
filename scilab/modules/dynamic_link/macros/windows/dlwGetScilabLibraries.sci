// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function scilablibrarieslist = dlwGetScilabLibraries()

  scilablibrarieslist = ["MALLOC.lib"; ..
                        "blasplus.lib"; ..
                        "libf2c.lib"; ..
                        "core.lib"; ..
                        "core_f.lib"; ..
                        "lapack.lib"; ..
                        "libintl.lib"; ..
                        "hashtable.lib"; ..
                        "intersci.lib"; ..
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
                        "scilab_windows.lib"]; 

endfunction
//=============================================================================