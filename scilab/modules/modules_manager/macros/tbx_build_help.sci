// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Simple wrapper around xmltojar

function tbx_build_help(moduletitle, path)

    rhs = argn(2);

    // Number of input arguments

    if and(rhs<> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "tbx_build_help", 1, 2));
    end

    // Input argument N°1
    // → name

    if type(moduletitle) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "tbx_build_help", 1));
    end

    if size(moduletitle,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), "tbx_build_help", 1));
    end

    // Input argument N°2
    // → path

    if rhs < 2 then
        path = pwd();
    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "tbx_build_help", 2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), "tbx_build_help", 2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"), "tbx_build_help", path));
        end
    end

    path = pathconvert(path, %f, %t);

    // Check scilab mode
    if and(getscilabmode() <> ["STD" "NW"]) then
        error(msprintf(gettext("%s: documentation cannot be built in this scilab mode: %s.\n"),"tbx_build_help",getscilabmode()));
    end

    directory_language = basename(path);
    default_language = "en_US"

    xmltojar(path, moduletitle, directory_language, default_language);

endfunction
