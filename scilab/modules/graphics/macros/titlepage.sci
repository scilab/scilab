// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1989 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
