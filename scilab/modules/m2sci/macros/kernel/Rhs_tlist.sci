// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
