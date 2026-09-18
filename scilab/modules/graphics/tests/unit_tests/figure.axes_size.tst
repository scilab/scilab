// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechalinria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// helper to execute a Scilab command after all pending AWT events have been processed
jimport org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement

// Test 1 : auto_resize = "on"
// ===========================

// Create a default figure
my_figure = scf();
clf(my_figure,"reset");

// Get default values
my_default_figure_size = my_figure.figure_size;
my_default_axes_size   = my_figure.axes_size;

// auto_resize = "on"
my_figure.auto_resize = "on";
ScilabInterpreterManagement.executeAndWaitForEDT("my_figure.axes_size = my_default_axes_size + 10;");
assert_checkequal(my_figure.figure_size, my_default_figure_size + 10);
assert_checkequal(my_figure.axes_size, my_default_axes_size + 10);

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
ScilabInterpreterManagement.executeAndWaitForEDT("my_figure.axes_size = my_default_axes_size + 10;");
assert_checkequal(my_figure.figure_size, my_default_figure_size);
assert_checkequal(my_figure.axes_size, my_default_axes_size + 10);

// Delete
delete(my_figure);
