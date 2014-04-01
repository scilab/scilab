//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
