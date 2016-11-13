// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Based on E.Segre dynamickeywords.sce
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function list_keywords = getscilabkeywords()

    scilab_primitives = [];
    scilab_commands = [];
    predef_variables = [];
    scilab_functions = [];
    scicos_functions = [];

    [scilab_primitives, scilab_commands] = what();

    // predefined variables
    predef_variables = predef("names");

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
