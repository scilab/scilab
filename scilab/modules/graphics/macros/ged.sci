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

function ged(k,win)

    if getos() == "Darwin" then
        error(msprintf(gettext("The ged function is not available on MacOSX.")));
    end

    if ~%tk then
        warning(msprintf(gettext("Tcl/Tk interface not installed.")));
        return
    end

    // Check number of arguments
    if argn(2) ==1 then
        win=get(gcf(),"figure_id")
    elseif argn(2)<>2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "ged", 2));
    end

    // Check argument #1
    if typeof(k) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A Real expected.\n"), "ged", 1));
    end
    if size(k, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A Real expected.\n"), "ged", 1));
    end

    // Check argument #2
    if typeof(win) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A Real expected.\n"), "ged", 2));
    end
    if size(win, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A Real expected.\n"), "ged", 2));
    end

    global ged_current_figure
    global ged_cur_fig_handle

    ged_current_figure=gcf(); //Preserve current figure

    ged_cur_fig_handle=scf(win);
    show_window(ged_cur_fig_handle);

    // for TCL input args built with string(). See bug http://bugzilla.scilab.org/2479
    initFormat = format()
    format("v",18)  // To be restored with initFormat before leaving

    if k>3 then
        TCL_EvalStr("set isgedinterp [interp exists ged]")
        if ~TCL_ExistInterp( "ged" ) then
            TCL_CreateSlave( "ged" ) ;
        end

    end

    select k
    case 1 then //Select (make it current)
        format(initFormat(2),initFormat(1))
        return
    case 2 then //redraw
        // nothing to do in new graphic mode
    case 3 then //erase
        clf()
    case 4 then //copy
        ged_copy_entity()
    case 5 then //past
        ged_paste_entity()
    case 6 then //move
        ged_move_entity()
    case 7 then //Delete Entity
        ged_delete_entity()

    case 8 then //edit current figure properties

        // hierarchical viewer
        TK_send_handles_list(ged_cur_fig_handle)
        TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(ged_cur_fig_handle)))

        //color_map array for color sample display
        f=ged_cur_fig_handle;
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

        ged_figure(ged_cur_fig_handle)
    case 9 then //edit current axes

        // hierarchical viewer
        TK_send_handles_list(ged_cur_fig_handle)
        TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(gca())))

        //color_map array for color sample display
        f=ged_cur_fig_handle;
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

        ged_axes(gca())
    case 10 then //start Entity picker
        fig=ged_cur_fig_handle
        fig_ud=get(fig,"user_data")
        if fig.event_handler<>""& fig.event_handler<>"ged_eventhandler" then
            //push current event handler in fig user data if possible
            if fig_ud==[] then fig_ud=struct();end
            if typeof(fig_ud)=="st" then
                if ~isfield(fig_ud,"handlers") then  fig_ud.handlers=[],end
                fig_ud.handlers=[fig_ud.handlers;
                fig.event_handler fig.event_handler_enable]
                set(fig,"user_data",fig_ud)
            else
                warning(_("Entity picker cannot be enabled, user data figure field is already used" ))
                return
            end
            fig.event_handler_enable = "off" //to prevent against bug 7855
            fig.event_handler="ged_eventhandler"
            fig.event_handler_enable="on"
        end
        ged_cur_fig_handle.info_message=_("Left click on a graphic entity to open its property editor");
    case 11 then //stop Entity picker
        fig=ged_cur_fig_handle
        fig_ud=get(fig,"user_data")
        seteventhandler("")
        fig.event_handler_enable = "off"
        if typeof(fig_ud)=="st"&isfield(fig_ud,"handlers")&fig_ud.handlers<>[] then
            fig.event_handler=fig_ud.handlers($,1)
            fig.event_handler_enable=fig_ud.handlers($,2)
            fig_ud.handlers= fig_ud.handlers(1:$-1,:)
            set(fig,"user_data",fig_ud)
        else
            fig.event_handler_enable = "off"
        end
    end
    scf(ged_current_figure)
    format(initFormat(2), initFormat(1))
endfunction
