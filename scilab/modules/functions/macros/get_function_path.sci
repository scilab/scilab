// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function path = get_function_path(name)

    path = [];
    rhs = argn(2);

    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "get_function_path", 1));
    end

    if type(name) <> 10 then
        error(999,msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"get_function_path",1));
    end

    if size(name,"*") <> 1 then
        error(999,msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"get_function_path",1));
    end

    libname = whereis(name);

    if libname <> [] then
        for i = 1:size(libname,"*")
            [funcnames, pathlib] = libraryinfo(libname(i));
            path = [path ; fullfile(pathlib, name + ".sci")];
        end
        path = pathconvert(path,%F);
    end

endfunction
