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


