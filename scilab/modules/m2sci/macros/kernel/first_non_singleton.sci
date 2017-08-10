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

function dim=first_non_singleton(m2scitlist)
    dim=-1
    if is_a_scalar(m2scitlist) | is_a_vector(m2scitlist) then
        dim=0;
        return
    end
    for k=1:size(m2scitlist.dims)
        if m2scitlist.dims(k)==0 then
            dim=0
            break
        elseif m2scitlist.dims(k)>0 & m2scitlist.dims(k)<>1 then
            dim=k
            break
        end
    end

endfunction
