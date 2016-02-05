// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=square(xmin,ymin,xmax,ymax)

    //definit un  environnement  graphique
    //permettant un trace isometrique.
    //
    //xmin,ymin, (xmax,ymax) specifient les abscisses et ordonnees minimum
    //           (maximum) du dessin. defaut (-2,-2,2,2)
    //
    //square modifie le facteur d'echelle sur l'axe  Ox. Pour conserver la
    //meme taille de dessin, utiliser plutot la macro isoview. Les extrema
    //sont imposes.
    //!

    [lhs,rhs]=argn(0);

    if (rhs>0) & (rhs<>4) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"square",0,4));
    end

    if rhs<=0 then
        xmin  = -2;
        ymin  = -2;
        xmax  =  2;
        ymax  =  2;
    end

    my_figure           = gcf();
    my_figure.axes_size = max(my_figure.axes_size) * ones(1,2);
    plot2d(0,0,1,"010"," ",[xmin,ymin,xmax,ymax]);

endfunction
