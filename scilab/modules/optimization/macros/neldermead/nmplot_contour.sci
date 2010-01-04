// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
  stepx = (xmax - xmin)/nx
  xdata = xmin:stepx:xmax;
  stepy = (ymax - ymin)/ny
  ydata = ymin:stepy:ymax;
  for ix = 1:length(xdata)
    for iy = 1:length(ydata)
      experiment = [xdata(ix) ydata(iy)];
      [ this.nmbase , fiexp ] = neldermead_function ( this.nmbase , experiment );
      zdata ( ix , iy ) = fiexp;
    end
  end
endfunction

