// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2008-2010 - Yann COLLETTE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function plist = add_param(list_name, param_name, param_value)

    if nargout == 2 then
        warning(sprintf(_("Second output argument of %s is obsolete.\n"), "add_param"))
        warning(sprintf(_("This argument will be permanently removed in Scilab %s"), "5.5.1"))
    end

    if nargin <> 2 & nargin <> 3 then
        error(sprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "add_param", 2, 3));
    end

    if typeof(list_name) == "plist" then
        if ~is_param(list_name, param_name) then
            list_name = setfield(1, [getfield(1, list_name) param_name], list_name);
            if nargin == 3 then
                list_name(param_name) = param_value;
            end
        else
            error(sprintf(gettext("%s: Wrong value for input argument #%d: key ""%s"" already defined.\n"), "add_param", 2, param_name));
        end
    else
        error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "add_param", 1, "plist"));
    end
    plist = list_name;

endfunction
