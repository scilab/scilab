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

function rhs_tlist=Rhs_tlist(varargin)
    // Create a new rhs tlist

    expressions=list()
    if size(varargin)==1 then
        if or(typeof(varargin(1))==["string","constant"]) then
            expressions(1)=Cste(varargin(1))
        else
            expressions(1)=varargin(1)
        end
    else
        for k=1:size(varargin)
            if or(typeof(varargin(k))==["string","constant"]) then
                expressions(k)=Cste(varargin(k))
            elseif varargin(k)<>list() then //list() is used for rand()...
                expressions(k)=varargin(k)
            end
        end
    end
    rhs_tlist=expressions

endfunction
