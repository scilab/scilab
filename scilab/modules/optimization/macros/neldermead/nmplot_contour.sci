// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
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
// nmplot_contour --
//   Plot the contours of the cost function.
//   The cost function must be a function with two parameters.
// Arguments
//   xmin , xmax , ymin , ymax : the bounds for the contour plot
//   nx , ny : the number of points in the directions x, y
//   xdata , ydata , zdata : vectors of data, as required by the contour command
//
function [ this , xdata , ydata , zdata ] = nmplot_contour ( this , xmin , xmax , ymin , ymax , nx , ny )
    // Check that there are only 2 parameters
    n = neldermead_cget ( this.nmbase , "-numberofvariables" );
    if n <> 2 then
        errmsg = msprintf(gettext("%s: Unexpected number of variables %d. Cannot draw contour plot for functions which do not have two parameters."),"nmplot_contour",n)
        error(errmsg)
    end
    xdata = linspace(xmin,xmax,nx);
    ydata = linspace(ymin,ymax,ny);
    for ix = 1:length(xdata)
        for iy = 1:length(ydata)
            experiment = [xdata(ix) ydata(iy)]';
            [ this.nmbase , fiexp ] = neldermead_function ( this.nmbase , experiment );
            zdata ( ix , iy ) = fiexp;
        end
    end
endfunction

