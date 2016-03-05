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

function whereami()
    // whereami - display calling tree
    //!
    [linn,mac]=where()
    nn=prod(size(linn))
    lm=max(length(mac))
    txt=[]
    for k=2:nn
        if mac(k)=="exec" then
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
    end
    write(%io(2),txt)
endfunction
