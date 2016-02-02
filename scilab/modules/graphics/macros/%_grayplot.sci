//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
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
//

//
// This is the demonstration script of grayplot
// used when calling grayplot without any parameter.
//

function [] = %_grayplot()
    t=-%pi:0.05:%pi;
    m=sin(t)'*cos(t);
    grayplot(t,t,m);
    f = gcf();
    my_plot_desc  = "grayplot";
    f.figure_name = my_plot_desc;
    xtitle(my_plot_desc," "," "," ");
    f.color_map = hotcolormap(128);
endfunction
