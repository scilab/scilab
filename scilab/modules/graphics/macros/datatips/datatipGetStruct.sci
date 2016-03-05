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

function t=datatipGetStruct(curve)

    warnobsolete(_("''datatips'' property"), "5.5.1")

    if type(curve)<>9|size(curve,"*")<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: Graphic handle expected.\n"),"datatipGetStruct",1))
    end

    t = curve.datatips;
endfunction
