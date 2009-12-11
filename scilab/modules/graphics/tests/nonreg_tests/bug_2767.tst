// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
// <-- Non-regression test for bug 2767-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2767
//
// <-- Short Description -->
// By default graphic windows are opened one above each others whereas they should be tiled.
// 

// create ten figures
for i=1:10,
  fig(i) = scf(i);
end;

// check that all figure have different positions from the previous one
for i=1:9,
  disp(fig(i).figure_position == fig(i+1).figure_position);
  if (fig(i).figure_position == fig(i+1).figure_position) then pause; end
end
