// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
