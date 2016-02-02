// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Color,CurColor]=setDefaultColor(CurColor)

    // Adds a quadruple to the list of elementary plots to do

    MAXCOL = 8;

    ColorTab=[0         0    1.0000;
    0    0.5000         0;
    1.0000         0         0;
    0    0.7500    0.7500;
    0.7500         0    0.7500;
    0.7500    0.7500         0;
    0.2500    0.2500    0.2500];


    CurColor = CurColor+1;

    if(modulo(CurColor,MAXCOL)==0)
        CurColor=1;
    else
        CurColor=modulo(CurColor,MAXCOL);
    end

    //disp('ColorTab(CurColor,:)=')
    //disp(ColorTab(CurColor,:))
    Color=addcolor(ColorTab(CurColor,:));


endfunction



