// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


// when clicking on Ticks button : REload ticks is called

function ReLoadTicks2TCL(h)
    global ged_handle;ged_handle=h;

    TCL_SetVar("Xaxes_visibleToggle",h.axes_visible(1))
    TCL_SetVar("Yaxes_visibleToggle",h.axes_visible(2))
    TCL_SetVar("Zaxes_visibleToggle",h.axes_visible(3))

    ticks = h.x_ticks;
    sizeticks = size(ticks.locations,1);
    if (sizeticks <> 0)
        TCL_EvalStr("unset LOCATIONS_X");
        TCL_EvalStr("unset LABELS_X");
    end

    ticks = h.y_ticks;
    sizeticks = size(ticks.locations,1);
    if (sizeticks <> 0)
        TCL_EvalStr("unset LOCATIONS_Y");
        TCL_EvalStr("unset LABELS_Y");
    end

    ticks = h.z_ticks;
    sizeticks = size(ticks.locations,1);
    if (sizeticks <> 0)
        TCL_EvalStr("unset LOCATIONS_Z");
        TCL_EvalStr("unset LABELS_Z");
    end

    TCL_SetVar("SubticksEntryX",string(h.sub_ticks(1)))
    //  TCL_GetVar("SubticksEntryX")

    // disp("h.sub_ticks(1) =")
    // disp(h.sub_ticks(1));

    TCL_SetVar("SubticksEntryY",string(h.sub_ticks(2)))

    select h.view
    case "2d"
        drawlater();
        h.view="3d"
        TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
        h.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
    end

    TCL_SetVar("XautoticksToggle",h.auto_ticks(1))
    TCL_SetVar("YautoticksToggle",h.auto_ticks(2))
    TCL_SetVar("ZautoticksToggle",h.auto_ticks(3))

    //ticks value: X axis
    ticks = h.x_ticks;

    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_x",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_X("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        //      disp("i vaut:");
        //      disp(i);
        val= "LABELS_X("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

    //ticks value: Y axis
    ticks = h.y_ticks;
    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_y",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_Y("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        val= "LABELS_Y("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

    //ticks value: Z axis
    ticks = h.z_ticks;
    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_z",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_Z("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        val= "LABELS_Z("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

endfunction
