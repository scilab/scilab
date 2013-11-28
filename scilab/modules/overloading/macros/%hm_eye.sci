// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M=%hm_eye(varargin)

    if size(varargin) <> 1 then
        error(msprintf(_("%s: Wrong number of input argument: %d to %d expected.\n"), "eye", 1, 2));
    end
    if typeof(varargin(1)) <> "hypermat" then
        error(msprintf(_("%s: Wrong type for input argument #%d: Hypermatrix expected.\n"), "eye", 1));
    end

    M = zeros(varargin(1));
    M(sub2ind(size(varargin(1)), (1:min(size(varargin(1))))' .*. ones(1,ndims(varargin(1))))) = 1;

endfunction
