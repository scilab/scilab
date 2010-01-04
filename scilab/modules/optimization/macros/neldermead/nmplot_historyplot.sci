// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_historyplot --
//  Plots the history from the given data file
// Arguments
//  this : the current nmplot object
//  datafile : the data file which contains the history
//  mytitle , myxlabel , myylabel : the parameters of the plot
//
function nmplot_historyplot ( this , datafile  , mytitle , myxlabel , myylabel )
  if (~isdef('datafile','local')) then
    datafile = this.foptfn;
  end
  if (~isdef('mytitle','local')) then
    mytitle = "";
  end
  if (~isdef('myxlabel','local')) then
    myxlabel = "";
  end
  if (~isdef('myylabel','local')) then
    myylabel = "";
  end
  exec(datafile,-1);
  nbiter = size ( history , 1 )
  plot2d ( history(1:nbiter,1) , history(1:nbiter,2) )
  f = gcf();
  f.children.title.text = mytitle;
  f.children.x_label.text = myxlabel;
  f.children.y_label.text = myylabel;
endfunction

