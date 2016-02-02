// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2009-2010 - DIGITEO - Yann COLLETTE
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [result,err] = get_param(list_name,param_name,param_default)
    [nargout,nargin] = argn();
    if ~isdef("param_default","local") then
        param_default = [];
    end

    if ( or(type(param_default) == [13 11]) ) then
        prot=funcprot()
        funcprot(0);
    end
    result = param_default;
    if ( or(type(param_default) == [13 11]) ) then
        funcprot(prot);
    end

    if typeof(list_name)=="plist" then
        if is_param(list_name,param_name) then
            if ( or(type(param_default) == [13 11]) ) then
                prot=funcprot()
                funcprot(0);
            end
            result = list_name(param_name);
            if ( or(type(param_default) == [13 11]) ) then
                funcprot(prot);
            end
            if nargout==2 then
                err = %F;
            end
        else
            if nargout==2 then
                err = %T;
            else
                warning(sprintf(gettext("%s: parameter %s not defined"),"get_param",param_name));
            end
        end
    else
        if nargout==2 then
            err = %T;
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "get_param", 1, "plist"));
        end
    end
endfunction
