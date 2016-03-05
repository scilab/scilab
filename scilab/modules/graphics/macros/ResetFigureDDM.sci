// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// Reset the Default Drawing Mode (DDM) of the figure
// immediate_drawing is set to its input value.
// This function is intended to be private.
function ResetFigureDDM(cur_figure, cur_draw_mode)

    if type(cur_figure) == 9
        if cur_figure.type == "Figure"
            cur_figure.immediate_drawing = cur_draw_mode;
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: figure graphic handle expected.\n"),"ResetFigureDDM",1));
            return;
        end
    end

endfunction
