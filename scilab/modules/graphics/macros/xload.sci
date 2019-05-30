// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function xload(fil,num)

    if and(argn(2)<>[1 2]) then
        msg = _("%s: Wrong number of input argument(s): %d or %d expected.\n");
        error(msprintf(msg, "xload", 1, 2));
    end

    if ~isfile(fil) then
        error(msprintf(gettext("%s: file %s does not exist.\n"), "xload", fil));
    end

    targetFigAlreadyExists = %f
    wins = winsid();
    cnum = [];          // id of the current figure
    if  wins~=[]
        cnum = gcf().figure_id
    end
    if  argn(2)==2 then
        if or(num==wins)
            targetFigAlreadyExists = %t
        end
    else
        if cnum~=[]
            num = cnum
            targetFigAlreadyExists = %t
        else
            num = 0
        end
    end
    if ~targetFigAlreadyExists then
        h = scf(num)
        uid = h.uid
        xloadFigure(fil)
        if gcf().uid~=uid   // load() has opened a new figure
            xdel(h.figure_id)
            gcf().figure_id = num
        end
        return
    end

    // The target figure already exists
    // ================================
    // The target figure may be empty (only a default axes). In this case,
    //  - the loaded figure must impose its properties to the existing target.
    //  - the default axes will have to be deleted
    %__f__= scf(num);  // target figure
    // Is the target figure blank?
    targetFigWasBlank = length(%__f__.children)==1 && %__f__.children.type=="Axes" && ..
                        length(%__f__.children.children)==0
    // Loading the source figure
    xloadFigure(fil)
    loadedFig = gcf();

    // Copying main source figure properties to the target when required:
    if targetFigWasBlank
        props = "auto_resize,viewport,figure_name,info_message,color_map,"+..
                "pixel_drawing_mode,anti_aliasing,background,rotation_style,"+..
                "event_handler_enable,event_handler,user_data,resizefcn,"+..
                "closerequestfcn,resize,toolbar_visible,menubar_visible,"+..
                "infobar_visible,layout,layout_options,icon,tag";
        // uncopied, imposed by the target:
        // figure_position, figure_size, axes_size, figure_id, immediate_drawing,
        // visible, toolbar, menubar, dockable, default_axes
        props = tokens(props,",")';
        for p = props
            %__f__(p) = loadedFig(p);
        end
    end

    // Copying contents:
    if loadedFig ~= %__f__ then
        for kC = size(loadedFig.children, "*"):-1:1
            copy(loadedFig.children(kC), %__f__); // Copy axes & uicontrols
        end
        delete(loadedFig); // Delete it, returned figure will be the one set as current by xload
    end
    if targetFigWasBlank then
        delete(%__f__.children($));
    end
    %__f__.visible="on"
    %__f__.immediate_drawing="on";
    scf(%__f__);
endfunction

function xloadFigure(fil)
    try
        load(fil)
    catch
        error(_("xload: Given file is not a graphic one.")) ;
    end
endfunction
