// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) INRIA - Sylvestre LEDRU
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function files=ls(varargin)

    // interface to ls

    opts=[];

    if size(varargin)==0 then
        path = "./";
    else
        path = varargin(1);
        if type(path) <> 10 then
            error(999,msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"ls",1));
        end
        for k=2:size(varargin)
            opts = [opts varargin(k)];
        end
    end

    if size(opts,"*")<>0 then
        warning(msprintf(gettext("%s: Options ignored.\n"),"ls"));
    end

    if or(path=="PWD") then path=evstr(path),end

    // dir returns names without the dirname
    files = listfiles(path);

endfunction
