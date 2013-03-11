// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function l=lstsize(varargin)
    [lhs, rhs] = argn(0);
    if lhs <> 1 then
        error(999, msprintf(_("%s: Wrong number of output arguments: %d expected.\n"), "lstsize", 1));
    end

    if rhs <> 1 then
        error(999, msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "lstsize", 1));
    end


    var = varargin(1);
    if type(var) <> 15 & type(var) <> 17 then
        error(999, msprintf(_("%s: Wrong type for argument %d: List expected.\n"), "lstsize", 1));
    end

    l = size(var);
endfunction
