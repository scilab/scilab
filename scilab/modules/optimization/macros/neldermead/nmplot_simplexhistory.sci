// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_simplexhistory --
//  Plots the simplex history on the current graphic window.
// Arguments
//  this : the current nmplot object
//
function nmplot_simplexhistory ( this , colorforeground , markforeground , markstyle )
  if (~isdef('foregroundcolor','local')) then
    colorforeground = 5;
  end
  if (~isdef('markforeground','local')) then
    markforeground = 3;
  end
  if (~isdef('markstyle','local')) then
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
endfunction

