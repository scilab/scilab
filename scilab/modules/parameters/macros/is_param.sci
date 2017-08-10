// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO 2008-2010 - Yann COLLETTE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [result, err] = is_param(list_name, param_name)

    [nargout, nargin] = argn();

    if nargin <> 2 then
        error(sprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "is_param", 2));
    end

    result = [];
    if typeof(list_name) == "plist" then
        if typeof(param_name) == "string" then
            result_vec = getfield(1, list_name) == param_name;
            result = or(result_vec(2:$));
            if nargout == 2 then
                err = %F;
            end
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "is_param", 2, "String"));
        end
    else
        if nargout == 2 then
            err = %T;
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "is_param", 1, "plist"));
        end
    end

endfunction
