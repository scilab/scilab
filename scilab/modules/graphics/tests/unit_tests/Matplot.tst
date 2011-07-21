// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// non regression bug for Matplot_properties
// test matplot object

// Check each default property
Matplot([1 2 3;4 5 6])
e = gce();

assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.visible, "on")
assert_checkequal(e.data, [1, 2, 3 ; 4, 5, 6])
assert_checkequal(e.clip_state, "clipgrf")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])

// used to crash Scilab
clf();
Matplot(1:32);

