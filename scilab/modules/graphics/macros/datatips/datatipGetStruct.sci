// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function t=datatipGetStruct(curve)

    warnobsolete(_("''datatips'' property"), "5.5.1")

    if type(curve)<>9|size(curve,"*")<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: Graphic handle expected.\n"),"datatipGetStruct",1))
    end

    t = curve.datatips;
endfunction
