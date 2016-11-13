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

function datatipRadioCallback(key,v)
    global datatipGUIHandles datatipGUICurve
    Keys=["sty" "lab" "box" "pos" "mul"]
    i=find(key==Keys)
    if v then
        datatipGUIHandles(i,1).value=1
        datatipGUIHandles(i,2).value=0
    else
        datatipGUIHandles(i,1).value=0
        datatipGUIHandles(i,2).value=1
    end
    if i<=3 then
        ud=datatipGetStruct(datatipGUICurve)
        style=ud.style
    end
    select key
    case "sty"
        style(1)=bool2s(~v)+1
    case "lab"
        style(3)=bool2s(v)
    case "box"
        style(2)=bool2s(v)
    case "pos"
        datatipSetInterp(datatipGUICurve,~v);
    case "mul"
        datatipSetReplaceMode(datatipGUICurve,~v);
    end

    if i<=3 then
        ud.style=style
        datatipSetStruct(datatipGUICurve,ud)
        fig=datatipGUICurve.parent
        while fig.type<>"Figure" then fig=fig.parent,end
        id=fig.immediate_drawing;
        fig.immediate_drawing="off"
        for i=1:size(ud.tips.children,"*")
            datatipSetTipStyle(ud.tips.children(i),style)
        end
        fig.immediate_drawing=id
    end
endfunction
