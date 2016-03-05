// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function varargout=%ce_size(M,job)
    // returns dimensions of a cell
    [lhs,rhs]=argn(0)
    dims=matrix(double(M.dims),1,-1);
    if rhs==2 then
        if job=="*" then
            varargout(1)=prod(dims)
            return
        elseif type(job)==1 then
            if size(job,"*") >1 then error(msprintf(_("%s: Wrong size for input argument #%d.\n"),"%ce_size",2));end
            if job<=0|job>size(dims,"*") then
                error(msprintf(_("%s: Wrong size for input argument #%d.\n"),"%ce_size",2));
            end
            varargout(1)=dims(job)
        else
            error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%ce_size",2));
        end
        return
    end
    if lhs==1 then
        varargout(1)=matrix(dims,1,-1)
    else
        if lhs>size(dims,"*") then error(msprintf(_("%s: Wrong number of output arguments.\n"),"%ce_size")),end
        for k=1:lhs
            varargout(k)=dims(k)
        end
    end
endfunction

