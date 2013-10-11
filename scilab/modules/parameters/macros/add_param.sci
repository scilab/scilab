// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [plist, err] = add_param(list_name, param_name, param_value)

    [nargout, nargin] = argn();

    if nargout == 2 then
        warning(sprintf(_("Second output argument of %s is obsolete.\n"), "add_param"))
        warning(sprintf(_("This argument will be permanently removed in Scilab %s"), "5.5.1"))
    end

    if nargin <> 2 & nargin <> 3 then
        error(sprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "add_param", 2, 3));
    end

    if typeof(list_name) == "plist" then
        if ~is_param(list_name, param_name) then
            setfield(1, [getfield(1, list_name) param_name], list_name);
            if nargout == 2 then
                err = %F;
            end
            if nargin == 3 then
                list_name(param_name) = param_value;
            end
        else
            error(sprintf(gettext("%s: Wrong value for input argument #%d: key ""%s"" already defined.\n"), "add_param", 2, param_name));
        end
    else
        if nargout == 2 then
            err = %T;
        end
        error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "add_param", 1, "plist"));
    end
    plist = list_name;

endfunction
