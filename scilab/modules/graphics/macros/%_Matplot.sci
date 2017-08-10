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
// This is the demonstration script of Matplot
// used when calling Matplot without any parameter.

function %_Matplot()
    ny = 400;
    nx = 300;
    M = pmodulo((0:(ny-1))'*ones(1:nx) + ones(ny,1)*(0:nx-1),ceil(max(nx,ny)/3));

    Matplot(M)
    isoview()

    f = gcf();
    my_plot_desc  = _("Classical Matplot");
    f.figure_name = my_plot_desc;
    f.color_map = coolcolormap(nx);

    a = gca();
    a.tight_limits = "on";
    a.title.font_size = 2;
    Tlab = _("Matplot(M)  with  size(M)=>(%d,%d)\nThe color''s number of pixel(i,j)\n  = rounded value of M(i,j)");
    Xlab = _("For data-scaled axes, please use Matplot1(..)");
    Ylab = _("Axes are scaled with M''s indices");
    xtitle(msprintf(Tlab,ny,nx), Xlab, Ylab);
endfunction

