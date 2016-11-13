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

function cont=Contents(varargin)
    // Create a new inference tlist

    fields=["contents","index","data"]

    nargs=size(varargin)

    // Contents: all unknown
    if nargs==0 then
        cont=tlist(fields,list(),list())
    elseif nargs==2 then
        if typeof(varargin(1))<>"list" then
            error(msprintf(gettext("index must be a list instead of a: %s."),typeof(varargin(1))))
        end
        if typeof(varargin(2))<>"list" then
            error(msprintf(gettext("index must be a list instead of a: %s."),typeof(varargin(2))))
        end
        cont=tlist(fields,varargin(1),varargin(2))
    else
        error(gettext("Wrong number of inputs."));
    end
endfunction
