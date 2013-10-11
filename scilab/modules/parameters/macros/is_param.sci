// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
