// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
