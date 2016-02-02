// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Samuel GOUGEON
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
// This is the demonstration script of Matplot1
// used when calling Matplot1 without any parameter.
//

function %_Matplot1()
    nc = 200    // Number of colors
    np = 2      // Number of periods
    nx = 350
    ny = 300
    [X,Y] = meshgrid(-nx/2:nx/2, -ny/2:ny/2);
    R = sqrt(X.^2+Y.^2);
    R = (R-min(R))/(max(R)-min(R))*nc*np;
    R = 1+pmodulo(R,nc);
    xmin = -15, xmax = 40, ymin = -0.03, ymax = 0.01

    Matplot1(R,[xmin ymin xmax ymax]);

    f = gcf();
    my_plot_desc  = _("Classical Matplot1");
    f.figure_name = my_plot_desc;
    f.color_map = hsvcolormap(nc);

    a = gca()
    a.data_bounds=[xmin ymin ; xmax ymax];
    a.tight_limits = "on"
    a.axes_visible=["on" "on" "off"];
    a.title.font_size = 2
    a.x_label.font_size = 2
    Tlab = _("Matplot1(M, [ %g  %g  %g  %g ])  with  size(M)=>(%d,%d)\npixel(i,j) is drawn with color number int(M(i,j))");
    Xlab = _("Axes are scaled with [xmin  ymin  xmax  ymax] specified")
    xtitle(msprintf(Tlab,xmin,ymin,xmax,ymax,nx,ny),Xlab)
endfunction

