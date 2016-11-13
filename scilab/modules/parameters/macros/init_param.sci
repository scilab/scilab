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

function plist = init_param(varargin)
    param_name = [];
    param_val  = [];
    if (modulo(length(varargin),2)~=0) then
        error(sprintf(gettext("%s: Wrong number of input argument(s): An even number expected.\n"),"init_param"));
    end
    for i=1:2:length(varargin)
        if typeof(varargin(i))~="string" then
            error(sprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"init_param",i));
        end
    end

    plist = mlist(["plist"],[]);

    for i=1:2:length(varargin)
        plist = add_param(plist,varargin(i),varargin(i+1));
    end
endfunction
