// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_contour --
//   Computes data necessary to create a contour plot of the cost function.
//   The cost function must be a function with two parameters.
// TODO : move to optimbase_compcontour
// Arguments
//   xmin , xmax , ymin , ymax : the bounds for the contour plot
//   nx , ny : the number of points in the directions x, y
//   xdata , ydata , zdata : vectors of data, as required by the contour command
//
function [ this , xdata , ydata , zdata ] = neldermead_contour ( this , xmin , xmax , ymin , ymax , nx , ny )
  stepx = (xmax - xmin)/nx
  xdata = xmin:stepx:xmax;
  stepy = (ymax - ymin)/ny
  ydata = ymin:stepy:ymax;
  for ix = 1:length(xdata)
    for iy = 1:length(ydata)
      x = [xdata(ix) ydata(iy)]
      [ this , f ] = neldermead_function ( this , x );
      zdata ( ix , iy ) = f;
    end
  end
endfunction

