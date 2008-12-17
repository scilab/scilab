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
if my_figure.info_message <> '' then pause, end

// Test the allocation

my_figure.info_message = "TEST INFO MESSAGE";
if my_figure.info_message <> "TEST INFO MESSAGE" then pause, end

// try an invalid info message
execstr("my_figure.info_message = 12312",'errcatch','n');
if my_figure.info_message <> "TEST INFO MESSAGE" then pause, end

// Delete
delete(my_figure);
