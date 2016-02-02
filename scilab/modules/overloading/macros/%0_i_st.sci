// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function st=%0_i_st(i,void,st)
    if type(i)==10 then
        f=getfield(1,st);
        k=find(f(3:$)==i);
        if k<>[] then
            f(k+2)=[];
            st = setfield(k+2,null(),st);
            st = setfield(1,f,st);
        else
            error(msprintf(_("%s: Invalid index.\n"),"%0_i_st"));
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"),"%0_i_st",1));
    end
endfunction
