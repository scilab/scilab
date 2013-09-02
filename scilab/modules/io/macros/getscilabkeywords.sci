// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Based on E.Segre dynamickeywords.sce
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function list_keywords = getscilabkeywords()

    scilab_primitives = [];
    scilab_commands = [];
    predef_variables = [];
    scilab_functions = [];
    scicos_functions = [];

    [scilab_primitives, scilab_commands] = what();

    // predefined variables
    names = who("get");
    predef_variables = names(($-predef())+1:$);

    //library functions
    libvar = librarieslist();

    for i = 1:size(libvar,1)
        scilab_functions = [scilab_functions; libraryinfo(libvar(i))];
    end

    //scicos basic functions: read the lib
    if with_module("xcos") then
        // TO DO: changes lib names with reorganization
        scicos_functions = [libraryinfo("scicos_utilslib"); libraryinfo("scicos_autolib")];
    end

    list_keywords = list(scilab_primitives, scilab_commands, predef_variables, scilab_functions, scicos_functions);

endfunction
