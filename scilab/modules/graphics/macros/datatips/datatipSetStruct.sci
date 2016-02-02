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

function ok=datatipSetStruct(curve,t)
    if argn(2)<>2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),...
        "datatipSetStruct",2))
    end
    if type(curve)<>9|size(curve,"*")<>1|or(curve.type<>"Polyline") then
        error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
        "datatipSetType",1,"Polyline"))
    end

    ok=%t
    %datatips_i_s=generic_i_s
    %datatips_i_st=generic_i_st
    u_d=get(curve,"user_data")
    if u_d==[] then
        u_d.datatips=t
    else
        if typeof(u_d)=="st"  then
            u_d.datatips=t
        elseif typeof(u_d)=="datatips" //for compatibilty
            u.datatips=u_d
            u_d=u
        else
            ok=%f
            warning(msprintf(_("%s: Wrong type for curve user_data field: a struct expected.\n"),...
            "datatipSetStruct"))
        end
    end
    set(curve,"user_data",u_d)
endfunction
