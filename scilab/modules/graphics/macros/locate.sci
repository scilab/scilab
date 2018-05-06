// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [x, but] = locate(n, flag)
    //[x]=locate(n,flag)
    //fonction permettant d'obtenir les coordonnees  d'un ou plusieurs
    //points designes a l'aide de la souris sur un trace.
    //Les resultats ne sont significatifs que sur les axes lineaires.
    //x=locate(n) . Si n>0 retourne dans x(2,n) les coordonnees des  n
    //             points designes.
    //           . Si n<=0 retourne dans x les coordonnees des points
    //             designes jusqu'a l'indication de fin de saisie, ce
    //             dernier point n'etant pas retenu.
    //x=locate()  . equivault au cas n <= 0
    //si flag=1, on trace une croix sur chaque point saisi
    //
    //Sur les systemes ayant une souris a boutons, on designe un point
    //en cliquant sur un des boutons, la fin de saisie pour  n<=0  est
    //indiquee en cliquant sur le bouton de gauche.
    //!

    [lhs, rhs] = argn(0)
    but = []
    show_window();
    if rhs <= 1
        flag = 0;
    end
    if rhs==0
        n = -1;
    end

    ax = gca()

    // Saving current properties
    mark_style=ax.mark_style;
    mark_size=ax.mark_size;
    mark_size_unit=ax.mark_size_unit;
    // Temporary values
    ax.mark_style = 2;
    ax.mark_size = 0;
    ax.mark_size_unit = "tabulated"
    db = ax.data_bounds;

    idf = gcf().figure_id;
    x = [];
    i = 0;

    // Main selection loop
    while %t,
        while %t
            [ib, x1, y1, iwin] = xclick()
            if or(ib==[0:5 10 11 12]) then
                break
            end
            if ib==-1000 & iwin==idf then //the window has been closed
                if n>0
                    x = []
                    but = -1000
                end
                return
            end
        end
        if iwin==idf & x1>=db(1) & x1<=db(2) & y1>=db(3) & y1<=db(4)
            // We record (and mark) the point only if it is in the
            // current axes in the current figure:
            if ~or(ib==[2 5 12])
                if flag==1
                    xpoly(x1, y1, "marks")
                end
                x = [x, [x1; y1]];
                but = [but, ib]
                i = i+1;
            end
        end
        if (n>0 & i==n) | (n<=0 & or(ib==[2 5 12]))
            break
        end
    end

    // deleting crosses
    npt = size(x,2);
    if npt>0 & flag==1 then
      delete(ax.children(1:npt))
    end
    // Restoring input properties
    ax.mark_size_unit = mark_size_unit;
    ax.mark_style = mark_style;
    ax.mark_size = mark_size;
endfunction
