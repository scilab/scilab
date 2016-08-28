// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 8000 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8000
//
// <-- Short Description -->
// get(gcf(), "%s %s") crashed Scilab

f = gcf();
s = "%s";
for i = 1:20
    if i <> 1 then
        s = s + " %s";
    end
    assert_checkerror("get(f, s);", msprintf(gettext("Unknown property: %s.\n"), s));
end
close(f)
