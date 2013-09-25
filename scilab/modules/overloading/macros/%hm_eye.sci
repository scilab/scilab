// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M=%hm_eye(varargin)
    nv=size(varargin);
    for k=1:nv
        if (type(varargin(k)) <> 1 & type(varargin(k)) <> 8) | size(varargin(k), "*") <> 1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: An integer value expected.\n"), "eye", k))
        end
        dims(k)=int(varargin(k));
    end

    A=[1, dims(3:$)'];
    M = matrix(repmat(eye(dims(1), dims(2)), A), dims);
endfunction
