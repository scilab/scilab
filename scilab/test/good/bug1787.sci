// Non-regression test file for bug 1787
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 10/02/2006

mode(-1);
clear ;

titleStr  = "Scilab P0W3R" ;
xLabelStr = "XXX" ;
yLabelStr = "YYY" ;

xtitle("Scilab P0W3R","XXX","YYY",1) ;
a = gca() ;
title  = a.title   ;
xLabel = a.x_label ;
yLabel = a.y_label ;

resT = ( title.text  == titleStr  ) & ( title.fill_mode  == "on" ) ;
resX = ( xLabel.text == xLabelStr ) & ( xLabel.fill_mode == "on" ) ;
resY = ( yLabel.text == yLabelStr ) & ( yLabel.fill_mode == "on" ) ;

// check that a box is drawn around the labels
// and that they have the rigth text
if ( resT & resX & resY ) then
    affich_result(%T,1787);
else
    affich_result(%F,1787);
end

clear ;
