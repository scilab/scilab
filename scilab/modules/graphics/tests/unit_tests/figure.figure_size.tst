// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechalinria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->


// Test 1 : auto_resize = "on"
// ===========================

// Create a default figure
my_figure = scf();
clf(my_figure,"reset");
sleep(200);//Wait for MVC update
// Get default values
my_default_figure_size = my_figure.figure_size;
my_default_axes_size   = my_figure.axes_size;

// auto_resize = "on"
my_figure.auto_resize = "on";
my_figure.figure_size = my_default_figure_size + 200;
sleep(200);//Wait for MVC update
if or( my_figure.figure_size <> (my_default_figure_size + 200) ) then pause; end
if or( my_figure.axes_size   <> (my_default_axes_size   + 200) ) then pause; end

// Delete
delete(my_figure);


// Test 2 : auto_resize = "off"
// ============================

// Create a default figure
my_figure = scf();
clf(my_figure,"reset");

// Get default values
my_default_figure_size = my_figure.figure_size;
my_default_axes_size   = my_figure.axes_size;

// auto_resize = "on"
my_figure.auto_resize = "off";
my_figure.figure_size = my_default_figure_size + 200;
if or( my_figure.figure_size <> (my_default_figure_size + 200) ) then pause; end
if or( my_figure.axes_size   <> my_default_axes_size ) then pause; end

// Delete
delete(my_figure);
