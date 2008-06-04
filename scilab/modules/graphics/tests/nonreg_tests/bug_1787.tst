// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1787 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1787
//
// <-- Short Description -->
//    Scilab 4.0 RC1
//    The calling of procedure xtitle bugs when the drawing of boxes is asked.
//
//    Jacques-Deric

titleStr  = "Scilab P0W3R" ;
xLabelStr = "XXX" ;
yLabelStr = "YYY" ;

xtitle("Scilab P0W3R",'XXX','YYY',1) ;
a = gca() ;
my_title  = a.title   ;
xLabel    = a.x_label ;
yLabel    = a.y_label ;

// check that a box is drawn around the labels
// and that they have the rigth text

if ( my_title.text  <> titleStr  ) | ( my_title.fill_mode  <> 'on' ) then pause,end
if ( xLabel.text    <> xLabelStr ) | ( xLabel.fill_mode    <> 'on' ) then pause,end
if ( yLabel.text    <> yLabelStr ) | ( yLabel.fill_mode    <> 'on' ) then pause,end
