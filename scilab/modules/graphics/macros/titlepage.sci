// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1989 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function titlepage(str,win)
    //titlepage(str) genere une page de titres graphiques contenant la ma-
    //trice de chaines de caracteres str ecrite centree dans  la page avec
    //une taille de caractere maximale.
    //!
    [lhs,rhs]=argn(0)

    if ~isdef("str") then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "titlepage", 1, 2));
    end

    if type(str) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "titlepage", 1));
    end


    old=xget("window")
    if rhs==2 then xset("window",win);end
    plot2d([0,1],[0,1],[-1,-1],"022");
    xstringb(0,0,str,1,1,"fill");
    xset("window",old)
endfunction
