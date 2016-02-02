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

function [string_list,err] = list_param(list_name)
    [nargout,nargin] = argn();
    string_list = [];
    if typeof(list_name)=="plist" then
        string_list = getfield(1,list_name);
        string_list = string_list(2:$);
        if nargout==2 then err = %F; end
    else
        if nargout==2 then
            err = %T;
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "list_param", 1, "plist"));
        end
    end
endfunction
