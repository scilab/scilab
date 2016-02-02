// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// nmplot_simplexhistory --
//  Plots the simplex history on the current graphic window.
// Arguments
//  this : the current nmplot object
//
function nmplot_simplexhistory ( this , colorforeground , markforeground , markstyle )
    drawlater();
    if (~isdef("foregroundcolor","local")) then
        colorforeground = 5;
    end
    if (~isdef("markforeground","local")) then
        markforeground = 3;
    end
    if (~isdef("markstyle","local")) then
        markstyle = 9;
    end
    exec(this.simplexfn,-1);
    nbiter = length ( history )
    n = neldermead_cget ( this.nmbase , "-numberofvariables" )
    for iter = 1:nbiter
        simplex = history(iter)
        xcoords  = simplex(1:n+1,1)
        ycoords  = simplex(1:n+1,2)
        plot2d ( xcoords , ycoords )
    end
    f = gcf()
    for iter = 1:nbiter
        f.children(1).children(iter).children.foreground = colorforeground;
        f.children(1).children(iter).children.mark_foreground = markforeground
        f.children(1).children(iter).children.mark_style = markstyle;
        f.children(1).children(iter).children.closed = "on";
    end
    drawnow();
endfunction

