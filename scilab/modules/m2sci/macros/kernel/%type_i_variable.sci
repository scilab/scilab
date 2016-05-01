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

function var=%type_i_variable(field,inslist,var)

    if typeof(inslist)=="ce" then
        if field=="type" then
            var.infer=inslist
        else
            error(gettext("Not yet implemented."))
        end
    elseif typeof(inslist)=="st" then
        pause
    else
        if or(typeof(var.infer)==["ce","st"]) then
            if field=="type" then
                dims=list()
                for kd=1:size(var.infer.dims,"*")
                    dims(kd)=var.infer.dims(kd)
                end
                var.infer=Infer(dims,Type())
                var.infer.type=inslist
            else
                error(gettext("Not yet implemented."))
            end
        else
            if field=="type" then
                var.infer.type=inslist
            else
                error(gettext("Not yet implemented."))
            end
        end
    end
endfunction
