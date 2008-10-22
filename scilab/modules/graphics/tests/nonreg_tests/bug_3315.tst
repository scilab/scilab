// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3315 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3315
//
// <-- Short Description -->
// The font of axes title is not properly displayed. If I move the title, the font is properly drawn

// display title
my_axe                  = gca();
my_axe.title.text       = "My Beautiful Plot";
my_axe.title.font_size  = 5;

// have a look to the figure text should not be jaggy

// Now, move the title :

my_axe.title.position = [0.25,0.5];


