// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function outvar=GEDeditvar_get(winId)
    // Simple variable editor - import data
    // This file is part of sciGUI toolbox
    // Copyright (C) 2004 Jaime Urzua Grez
    // mailto:jaime_urzua@yahoo.com
    // rev. 0.1
    //
    // This program is free software; you can redistribute it and/or modify
    //it under the terms of the GNU General Public License as published by
    //the Free Software Foundation; either version 2 of the License, or
    //(at your option) any later version.

    //disp("Please wait...");
    outvar=[];
    base="sciGUITable(win,"+string(winId)+",data";
    varType=evstr(TCL_GetVar(base+",type)"));
    varni=evstr(TCL_GetVar(base+",ni)"));
    varnj=evstr(TCL_GetVar(base+",nj)"));
    for j=1:varnj,
        ww=[];
        for i=1:varni,
            q=TCL_GetVar(base+","+string(i)+","+string(j)+")");
            if (varType~=10) then
                if (varType==4) then
                    if ((q=="T")|(q=="t")) then
                        ww=[ww;%t];
                    else
                        ww=[ww;%f];
                    end
                else
                    ww=[ww;evstr(q)];
                end
            else
                ww=[ww;q];
            end
        end
        outvar=[outvar ww];
    end
endfunction
