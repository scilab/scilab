// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test matplot object

// Check each default property
x = -10:10;
y = -10:10;
z = rand(21,21);
grayplot(x,y,z);
e = gce();

assert_checkequal(e.type, "Grayplot")
assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.visible, "on")
assert_checkequal(e.data.x, x')
assert_checkequal(e.data.y, y')
assert_checkequal(e.data.z, z)
assert_checkequal(e.data_mapping, "scaled")
assert_checkequal(e.clip_state, "clipgrf")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])
