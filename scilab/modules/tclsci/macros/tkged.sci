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

function tkged()
    global ged_handle
    global ged_cur_fig_handle

    h=ged_handle

    // hierarchical viewer
    TK_send_handles_list(ged_cur_fig_handle)
    TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(h)))

    //color_map array for color sample display
    f=getparfig(h);
    for i=1:size(f.color_map,1)
        redname= "RED("+string(i)+")";
        TCL_EvalStr("set "+redname+" "+string(f.color_map(i,1)));
        grename= "GREEN("+string(i)+")";
        TCL_EvalStr("set "+grename+" "+string(f.color_map(i,2)));
        bluname= "BLUE("+string(i)+")";
        TCL_EvalStr("set "+bluname+" "+string(f.color_map(i,3)));
    end

    TCL_SetVar("msdos",string(getos() == "Windows")) // to know the OS

    // get the number of the window associated with ged
    TCL_SetVar("sciGedIsAlive",string(ged_cur_fig_handle.figure_id)) ;
    TCL_SetVar("SCIHOME", SCIHOME) // to know the home directory to put temporary files

    select h.type
    case "Polyline"
        ged_polyline(h)
    case "Rectangle"
        ged_rectangle(h)
    case "Axes"
        ged_axes(h)
    case "Label" // for now the labels are inside the axes (F.Leray 06.12.05)
        ged_axes(h.parent)
        if (h == h.parent.x_label)
            TCL_EvalStr("Notebook:raise $uf.n X");
        elseif (h == h.parent.y_label)
            TCL_EvalStr("Notebook:raise $uf.n Y");
        elseif (h == h.parent.z_label)
            TCL_EvalStr("Notebook:raise $uf.n Z");
        elseif (h == h.parent.title)
            TCL_EvalStr("Notebook:raise $uf.n Title");
        end
    case "Figure"
        ged_figure(h)
    case "Compound"
        ged_Compound(h)

    case "Plot3d"
        ged_plot3d(h)
    case "Fac3d"
        ged_fac3d(h)
    case "Text"
        ged_text(h)
    case "Legend"
        ged_legend(h)
    case "Arc"
        ged_arc(h)
    case "Segs"
        ged_segs(h)
    case "Champ"
        ged_champ(h)
    case "Fec"
        ged_fec(h)
    case "Grayplot"
        ged_grayplot(h)
    case "Matplot"
        ged_matplot(h)
    case "Axis"
        ged_axis(h)
    end
endfunction

