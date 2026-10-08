// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

function whereami()
    // whereami - display calling tree
    //!
    [linn,mac]=where()
    nn=prod(size(linn))
    lm=max(length(mac(mac <> "<console>" & mac <> "<callback>")))
    txt=[]
    k=2
    while k<=nn
        if (mac(k)=="<console>" | mac(k)=="<callback>") & k<nn then
            // a command run between two instructions: what it called, where
            // it comes from, and what was running then
            if mac(k+1)=="exec" | mac(k+1)=="execstr" then
                if mac(k)=="<console>" then
                    fmt=gettext("%s called from the console during line %s of %s")
                else
                    fmt=gettext("%s called by a callback during line %s of %s")
                end
                if mac(k+1)=="exec" then
                    running="exec_file"
                else
                    running="execstr"
                end
            else
                if mac(k)=="<console>" then
                    fmt=gettext("%s called from the console during line %s of macro %s")
                else
                    fmt=gettext("%s called by a callback during line %s of macro %s")
                end
                running=mac(k+1)
            end
            txt=[txt;
            msprintf(fmt,part(mac(k-1),1:lm), string(linn(k+1)), running)]
            k=k+1
        elseif mac(k)=="exec" then
            txt=[txt;
            msprintf(gettext("%s called at line %s of %s"),part(mac(k-1),1:lm), string(linn(k)), "exec_file")]
        elseif mac(k)=="execstr" then
            txt=[txt;
            msprintf(gettext("%s called at line %s of %s instruction."),part(mac(k-1),1:lm), string(linn(k)),"execstr")]
        elseif mac(k)=="pause" then
            txt=[txt;
            msprintf(gettext("%s called under %s"),part(mac(k-1),1:lm),"pause") ]
        else
            txt=[txt;
            msprintf(gettext("%s called at line %s of macro %s"), part(mac(k-1),1:lm), string(linn(k)),mac(k))]
        end
        k=k+1
    end
    write(%io(2),txt)
endfunction
