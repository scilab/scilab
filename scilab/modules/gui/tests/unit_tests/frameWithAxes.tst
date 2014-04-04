// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
f = figure("layout", "gridbag", "default_axes", "off", "position", [0, 0, 800, 800]);

for i = 1:2
    for j = 1:2
        frame = uicontrol(f, "style", "frame", ...
        "layout", "border", ...
        "constraints", createConstraints("gridbag", [i, j, 1, 1], [1,1], "both"));
        newaxes(frame);
        plot3d();
    end
end

assert_checkequal(size(f.children, "*"), 4);
for i = 1:4
    assert_checkequal(f.children(i).type, "uicontrol");
    assert_checkequal(f.children(i).style, "frame");
    assert_checkequal(f.children(i).children(1).type, "Axes");
    assert_checkequal(f.children(i).children(1).children(1).type, "Plot3d");
    assert_checkequal(f.children(i).position(3:4), [400, 400]);
end
