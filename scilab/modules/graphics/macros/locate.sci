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

function [x,but]=locate(n,flag)
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

    [lhs,rhs]=argn(0)
    but=[]
    show_window();
    if rhs<=1,flag=0;end
    if rhs==0;n=-1;end

    ax=gca()
    mark_style=ax.mark_style;mark_size=ax.mark_size;mark_size_unit=ax.mark_size_unit;
    ax.mark_style=2;ax.mark_size=0;ax.mark_size_unit = "tabulated"
    deff( "[]=clearmode(flag)",[
    "npt=size(x,2);"
    "if npt>0&flag==1 then"
    "  delete(ax.children(1:npt))"
    "end"
    "ax.mark_size_unit=mark_size_unit;"
    "ax.mark_style=mark_style;"
    "ax.mark_size=mark_size;"]);
    x=[];
    if n >= 0 then
        for i=1:n,
            while %t
                [ib,x1,y1]=xclick();
                if or(ib==[0:5 10 11 12]) then break,end
                if ib==-100 then return,end //the window has been closed
            end
            if flag==1,xpoly(x1,y1,"marks");end
            x=[x,[x1;y1]];
            but=[but,ib]
        end
    else
        while %t,
            while %t
                [ib,x1,y1]=xclick(),
                if or(ib==[0:5 10 11 12]) then break,end
                if ib==-100 then return,end //the window has been closed
            end
            if or(ib==[0 3 10]) then
                break //terminate the acquisition loop
            elseif flag==1 then xpoly(x1,y1,"marks"),end
            x=[x,[x1;y1]];
            but=[but,ib]
        end
    end
    clearmode(flag);
endfunction
