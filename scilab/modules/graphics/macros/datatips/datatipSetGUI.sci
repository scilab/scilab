// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function datatipSetGUI()
    global datatipGUIwin
    if datatipGUIwin==[] then return,end
    global datatipGUIHandles datatipGUICurve
    ud=datatipGetStruct(datatipGUICurve)
    if typeof(ud)<>"datatips" then
        //no datatips structure yet, initialize it
        ud=datatipGetStruct(curve)
    end
    style=ud.style
    if style(1)==1 then
        datatipGUIHandles(1,1).value=1
        datatipGUIHandles(1,2).value=0
    else
        datatipGUIHandles(1,1).value=0
        datatipGUIHandles(1,2).value=1
    end
    if style(3)==1 then
        datatipGUIHandles(2,1).value=1
        datatipGUIHandles(2,2).value=0
    else
        datatipGUIHandles(2,1).value=0
        datatipGUIHandles(2,2).value=1
    end
    if style(2)==1 then
        datatipGUIHandles(3,1).value=1
        datatipGUIHandles(3,2).value=0
    else
        datatipGUIHandles(3,1).value=0
        datatipGUIHandles(3,2).value=1
    end
    if ~ud.interpolate then
        datatipGUIHandles(4,1).value=1
        datatipGUIHandles(4,2).value=0
    else
        datatipGUIHandles(4,1).value=0
        datatipGUIHandles(4,2).value=1
    end
    if ~ud.replace then
        datatipGUIHandles(5,1).value=1
        datatipGUIHandles(5,2).value=0
    else
        datatipGUIHandles(5,1).value=0
        datatipGUIHandles(5,2).value=1
    end
endfunction


