// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
// Copyright (C) 2016, 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function isoview(varargin)
    // isoview
    // isoview on
    // isoview off
    // isoview(idGraphics [,"on"|"off"])

    rhs = argn(2)
    if rhs>2 then
        msg = _("%s: Wrong number of input argument(s): 0, 1, or 2 arguments expected.\n")
        error(msprintf(msg, "isoview"));
    end

    // isoview action to be performed: ""=switch, "on", "off"
    action = ""
    if rhs>0 & typeof(varargin($))=="string" then
        tmp = convstr(varargin($))
        if tmp(1)=="on"
            action = "on"
        elseif tmp(1)=="off"
            action = "off"
        end
    end

    // Set of axes to be processed
    // ---------------------------
    if rhs==0 | (rhs==1 & type(varargin(1))==10)
        a = gca()
    else // varargin(1) must be a graphics handle
        a = varargin(1)
        if type(a)~=9 then
            msg = _("%s: Wrong type for input argument #%d: Graphic handle or string expected.\n")
            error(msprintf(msg, "isoview", 1));
        end

        a0 = a  // vector of input handles
        a = []  // vector of axes to be processed
        for i=1:size(a0,"*")
            thisH = a0(i)
            if thisH.type=="Figure"
                // All the axes of the figure must be isoviewed
                tmp = thisH.children
                a = [a tmp(find(tmp.type=="Axes"))' ]
            elseif thisH.type=="Axes"
                a = [a thisH]
            else
                while thisH.type~="Axes" & thisH.type~="Figure"
                    thisH = thisH.parent
                end
                if thisH.type=="Axes"
                    a = [a thisH]
                end
                //no else: if thisH.type=="Figure" => no associated Axes
            end
        end
    end

    // Performing actions
    // ------------------
    for i = 1:size(a,"*")
        tmp = a(i).isoview
        if action=="on" | (action=="" & tmp=="off")
            a(i).cube_scaling = "off"
            a(i).isoview = "on"
        else
            a(i).cube_scaling = "on"
            a(i).isoview = "off"
        end
        a(i).clip_state = "clipgrf"
    end
endfunction

