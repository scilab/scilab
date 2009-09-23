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
  // Check that there are only 2 parameters
  n = neldermead_cget ( this , "-numberofvariables" );
  if n <> 2 then
    errmsg = msprintf(gettext("%s: Unexpected number of variables %d. Cannot draw contour plot for functions which do not have two parameters."),"nmplot_contour",n)
    error(errmsg)
  end
  stepx = (xmax - xmin)/nx
  xdata = xmin:stepx:xmax;
  stepy = (ymax - ymin)/ny
  ydata = ymin:stepy:ymax;
  // 1. Compute the matrix of experiments
  nexp = length(xdata) * length(ydata)
  iexp = 1;
  for ix = 1:length(xdata)
    for iy = 1:length(ydata)
      x(iexp,1:2) = [xdata(ix) ydata(iy)];
      iexp = iexp + 1;
    end
  end
  // 2. Perform the experiments
  // Vectorize the call to the cost function if possible
  vectorizefunction = neldermead_cget ( this , "-vectorizefunction" );
  if vectorizefunction == 0 then
    for iexp = 1:nexp
      [ this , f(iexp) ] = neldermead_function ( this , x(iexp,1:2) );
    end
  else
    [ this , f ] = neldermead_function ( this , x );
  end
  // 3. Store the experiments results in a matrix, suitable for input to the contour function
  iexp = 1;
  for ix = 1:length(xdata)
    for iy = 1:length(ydata)
      zdata ( ix , iy ) = f(iexp);
      iexp = iexp + 1;
    end
  end
endfunction

