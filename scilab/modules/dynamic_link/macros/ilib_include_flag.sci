// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Vincent LEJEUNE
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

function ret = ilib_include_flag(path_strings)

    if type(path_strings) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "ilib_include_flag", 1));
    end

    ret = "";

    if and(size(path_strings)<>1) then
        //path_strings is not a vector
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"), "ilib_include_flag", 1));
    else
        //change path_strings to a line vector to have ret as a line vector
        path_strings=path_strings(:)'
    end


    for path = path_strings
        if ~isdir(fullpath(path)) then
            error(msprintf(gettext("%s: Failed to add include path : %s"), "ilib_include_flag", path));
        end
        if (getos() == "Windows") then
            ret = ret + " -I""" + pathconvert(fullpath(path), %f) + """";
        else
            ret = ret + " -I" + pathconvert(fullpath(path), %f);
        end
    end
endfunction
