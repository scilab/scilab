// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
