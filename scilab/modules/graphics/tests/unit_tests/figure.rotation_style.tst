// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechalinria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Create a default figure
my_figure = scf();
clf(my_figure,"reset");

// Test the default value
if my_figure.rotation_style <> "unary" then pause, end

// Test the allocation
my_figure.rotation_style = "multiple";
if my_figure.rotation_style <> "multiple" then pause, end

execstr("my_figure.rotation_style = ''badvalue''",'errcatch','n');
if my_figure.rotation_style <> "multiple" then pause, end

my_figure.rotation_style = "unary";
if my_figure.rotation_style <> "unary" then pause, end

// Delete
delete(my_figure);
