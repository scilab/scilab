// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Vincent LEJEUNE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//=============================================================================

function ret = ilib_include_flag(path_strings)

    if type(path_strings) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "ilib_include_flag", 1));
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
