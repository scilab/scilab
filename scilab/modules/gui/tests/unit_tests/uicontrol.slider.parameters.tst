// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

exec(SCI+"/modules/gui/tests/unit_tests/uicontrol.slider.parameters.tst.sci");

// Parameters
// =========================================================================

global axes_w;
global axes_h;

global padding_x;
global padding_y;

axes_w      = 1100;    // Axes width
axes_h      = 600;     // Axes height

padding_x   = 10;      // Horizontal padding between each elements
padding_y   = 10;      // Vertical padding between each elements


// Figure creation
// =========================================================================

my_figure_handle                 = scf(100001);
my_figure_handle.background      = -2;
my_figure_handle.figure_position = [0 0];
my_figure_handle.axes_size       = [axes_w axes_h];
my_figure_handle.figure_name     = gettext("Test Slider [Parameters]");

// Title
// =========================================================================

title_1 = uicontrol(my_figure_handle                                              , ...
	"position"            , [padding_x axes_h-padding_y-30 axes_w-2*padding_x 30] , ...
	"Style"               , "text"                                                , ...
	"String"              , "Test Slider"                                         , ...
	"BackgroundColor"     , [1 1 1]                                               , ...
	"Fontsize"            , 16                                                    , ...
	"horizontalalignment" , "center"                                              , ...
	"tag"                 , "pos_title_1"                                         );

// Slider parameters
// =========================================================================

global text_w;
global text_h;
global text_x;

global slider_w;
global slider_h;
global slider_x;

global value_w;
global value_h;
global value_x;

global num_slider;

text_w     = 400;
text_h     = 20;
text_x     = padding_x;

slider_w   = 500;
slider_h   = text_h;
slider_x   = 2*padding_x+text_w;

value_w    = 100;
value_h    = text_h;
value_x    = 3*padding_x+text_w+slider_w;

num_slider = 0;

// Create Slider
// =========================================================================

create_slider(  0 ,  10 ,[1 2]   ,   5);
create_slider(  0 , 100 ,[10 20] ,  50);
create_slider(  0 , 100 ,[1 2]   ,  50);
create_slider(  0 ,1000 ,[1 2]   , 500);
create_slider(  0 , 500 ,[1 2]   , 500);
create_slider(-10 ,  10 ,[1 2]   ,   0);
create_slider(-10 ,  10 ,[1 2]   ,  10);
create_slider(-10 ,  10 ,[1 2]   ,  15);
create_slider(-10 ,  10 ,[1 2]   ,  20);
