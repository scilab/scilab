// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

