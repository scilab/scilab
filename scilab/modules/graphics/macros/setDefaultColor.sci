// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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



