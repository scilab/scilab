// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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
// nmplot_historyplot --
//  Plots the history from the given data file
// Arguments
//  this : the current nmplot object
//  datafile : the data file which contains the history
//  mytitle , myxlabel , myylabel : the parameters of the plot
//
function nmplot_historyplot ( this , datafile  , mytitle , myxlabel , myylabel )
    if (~isdef("datafile","local")) then
        datafile = this.foptfn;
    end
    if (~isdef("mytitle","local")) then
        mytitle = "";
    end
    if (~isdef("myxlabel","local")) then
        myxlabel = "";
    end
    if (~isdef("myylabel","local")) then
        myylabel = "";
    end
    exec(datafile,-1);
    nbiter = size ( history , 1 )
    plot2d ( history(1:nbiter,1) , history(1:nbiter,2) )
    xtitle(mytitle,myxlabel,myylabel)
endfunction

