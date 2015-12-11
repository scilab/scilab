// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function varargout=%st_size(M,job)

    // returns dimensions of an hyper matrix
    [lhs,rhs]=argn(0)
    dims=matrix(double(M.dims),1,-1)
    if rhs==2 then
        if job=="*" then
            varargout(1)=prod(dims)
            return
        elseif type(job)==1 then
            if size(job,"*") >1 then error(msprintf(_("%s: Wrong size for input argument #%d.\n"),"%st_size",2));end
            if job<=0|job>size(dims,"*") then
                error(msprintf(_("%s: Wrong size or value for input argument #%d.\n"),"%st_size",2));
            end
            varargout(1)=dims(job)
        else
            error(msprintf(_("%s: Wrong value for input argument #%d.\n"),"%st_size",2))
        end
        return
    end
    if lhs==1 then
        varargout(1)=matrix(dims,1,-1)
    else
        if lhs>size(dims,"*") then error(msprintf(_("%s: Wrong number of output arguments.\n"),"%st_size")),end
        for k=1:lhs
            varargout(k)=dims(k)
        end
    end
endfunction
