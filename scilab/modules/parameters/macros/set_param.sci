// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [plist,err] = set_param(list_name,param_name,param_value)
    [nargout,nargin] = argn();
    plist = list_name;
    if typeof(list_name)=="plist" then
        if nargout==2 then err = %F; end
        if is_param(plist,param_name) then
            plist(param_name) = param_value;
        else
            err = %T;
            warning(sprintf(gettext("%s: parameter doesn''t exist"),"set_param"));
        end
    else
        if nargout==2 then
            err = %T;
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "set_param", 1, "plist"));
        end
    end
endfunction
