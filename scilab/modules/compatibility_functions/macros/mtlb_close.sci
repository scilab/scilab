// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [status]=mtlb_close(h)
    // Emulation function for Matlab close()

    rhs=argn(2)

    warning(msprintf(gettext("%s: ''%s'' ignored.\n"),"mtlb_close", "status"));
    status=1

    // close
    if rhs<=0 then
        if type(gcf())==9 then
            delete(gcf())
        else
            close()
        end
        // close(h) - close('all') - close(name)
    elseif rhs==1 then
        if type(h)==9 then
            delete(h)
        elseif type(h)==1 then
            close(h)
        elseif type(h)==10 then
            if h=="all" then
                xdel(winsid())
            else // close(name)
                cf_save=gcf()

                allwin=winsid()
                for k=1:size(allwin,"*")
                    if get(scf(allwin(k)),"figure_name")==h then
                        delete(gcf())
                        break
                    end
                end

                scf(cf_save)
            end
        else // Unknown type for h
            error(msprintf(gettext("%s: This feature has not been implemented.\n"),"mtlb_close"));
        end
    else // close('all','hidden')
        warning(msprintf(gettext("%s: All windows deleted.\n"),"mtlb_close"));
        xdel(winsid())
    end
endfunction

