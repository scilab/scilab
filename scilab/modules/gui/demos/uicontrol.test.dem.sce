// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Parameters
// =========================================================================

page_w       = 970;     // Page width
page_h       = 600;     // Page height

margin_x     = 15;      // Horizontal margin between each elements
margin_y     = 15;      // Vertical margin between each elements

defaultfont  = "arial"; // Default Font

axes_w       = page_w + margin_x; // Axes width
axes_h       = page_h + margin_y; // Axes height

// Figure creation
// =========================================================================

my_test_fig = figure(99999);

my_test_fig.background      = -2;
my_test_fig.figure_position = [0 0];
my_test_fig.axes_size       = [axes_w axes_h];
my_test_fig.figure_name     = "SciView";

// Label
// =========================================================================

str = "";
str = str + "<html>";
str = str + "<img src=""file:///"+SCI+"/modules/gui/demos/test.png"" />";
str = str + "</html>";

my_text = uicontrol( ...
"parent"              , my_test_fig,...
"style"               , "text",...
"string"              , str,...
"units"               , "pixels",...
"position"            , [ margin_x margin_y page_w page_h ],...
"background"          , [1 1 1], ...
"tag"                 , "my_text" ...
);
