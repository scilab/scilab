// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
