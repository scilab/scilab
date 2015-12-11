// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
