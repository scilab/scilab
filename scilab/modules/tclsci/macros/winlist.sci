// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - Jaime Urzua
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function hout=winlist()
    // Return a list
    // This file is part of sciGUI toolbox
    // rev. 0.1
    //
    // This program is free software; you can redistribute it and/or modify
    //it under the terms of the GNU General Public License as published by
    //the Free Software Foundation; either version 2 of the License, or
    //(at your option) any later version.

    rhs=argn(2);
    if execstr("rep=TCL_GetVar(''sciGUITable(win,id)'')","errcatch")<>0 then
        hout=[];
        return
    end
    h1=evstr(rep);
    if (length(h1)==1) then
        h1=[];
    else
        h1=h1(2:$)';
    end
    hout=h1;
endfunction

