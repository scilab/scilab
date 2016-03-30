// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [fig]=get_figure_handle(index)

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "get_figure_handle", 1));
    end

    // check index
    figureIndices = winsid();
    // check that index is within the opened windows
    if (find(figureIndices == index) == []) then
        fig = [];
        return;
    end

    // return the figure handle from its index
    curFig = gcf();
    fig = scf(index);

    // scf sets the figure, so switch back to curFig
    scf(curFig);

endfunction

