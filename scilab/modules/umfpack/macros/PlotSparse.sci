//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [] = PlotSparse(A, style)
    //
    //  PURPOSE
    //     plot the pattern of non nul elements of a sparse matrix
    //
    //  ARGUMENTS
    //     A     : a sparse matrix
    //     style : (optional) a string given the color and/or the
    //             marker type of the form "[color][mark]" where
    //             color may be a number referring the color you
    //             want to use (in the current colormap). If you
    //             use the std colormap then color may be one of
    //             the following letters :
    //               k  for black       b  for blue
    //               r  for red         g  for green
    //               c  for cyan        m  for magenta
    //               y  for yellow      t  for turquoise
    //               G  a dark green
    //
    //             mark must be one of the following :
    //               .  point             +  plus
    //               x  cross             *  circled plus
    //               D  filled diamond    d  diamond
    //               ^  upper triangle    v  down triangle
    //               o  circle
    //
    //             by default you have "b." (in fact the 2d color) and
    //             this is also forced in case of error.
    //
    //  COMMENTS
    //     for "big" matrix use essentially point (.) as marker
    //
    //
    //  AUTHOR
    //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    //
    default_markColor = 2  // blue in std colormap
    default_markId    = 0  // a point .

    [lhs, rhs] = argn()

    if ( rhs<1 | rhs>2 ) then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"PlotSparse"));
    end

    if (typeof(A) == "sparse") then
        [m, n] = size(A)
        nel = nnz(A)
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: sparse matrix expected.\n"),"PlotSparse",1));
    end

    if rhs == 1 then
        markColor = default_markColor
        markId = default_markId
    elseif typeof(style) ~= "string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"PlotSparse",2));
    else
        [ markColor , markId ] = ana_style(style)
    end

    // display
    drawlater()
    ij = spget(A)
    plot2d(ij(:,2), ij(:,1),-markId)
    e = gce();
    e1 = e.children(1);
    e1.mark_foreground = markColor;

    ca=gca();
    ca.box="on";
    ca.axes_reverse(2)="on";
    mat_size = size(A);
    ca.data_bounds = [-.5, -.5 ; mat_size(2)+.5 , mat_size(1)+.5];
    ca.tight_limits="on";

    // information about nnz
    captions(e1, "nnz = " + string(nnz(A)), "lower_caption");

    drawnow()

endfunction
