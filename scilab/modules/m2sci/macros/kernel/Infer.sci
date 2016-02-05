// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function infer=Infer(varargin)
    // Create a new inference tlist

    fields=["infer","dims","type","contents"]

    rhs=argn(2)

    nargs=size(varargin)

    // Infer(): all unknown
    if nargs==0 then
        infer=tlist(fields,list(Unknown,Unknown),Type(Unknown,Unknown),Contents())
    elseif nargs==2 then
        if typeof(varargin(1))<>"list" then
            error(msprintf(gettext("dims must be a list instead of a: %s."),typeof(varargin(1))));
        end
        if typeof(varargin(2))<>"type" then
            error(msprintf(gettext("type must be a ''type'' tlist instead of a: %s."),typeof(varargin(2))));
        end
        infer=tlist(fields,varargin(1),varargin(2),Contents())
    elseif nargs==3 then // Should only be used for cells and structs
        if typeof(varargin(1))<>"list" then
            error(msprintf(gettext("dims must be a list instead of a: %s."),typeof(varargin(1))));
        end
        if typeof(varargin(2))<>"type" then
            error(msprintf(gettext("type must be a ''type'' tlist instead of a: %s."),typeof(varargin(2))));
        end
        if typeof(varargin(3))<>"contents" then
            error(msprintf(gettext("contents must be a ''contents'' tlist instead of a: %s."),typeof(varargin(3))));
        end
        infer=tlist(fields,varargin(1),varargin(2),varargin(3))
    end
endfunction
