// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for issue 16847 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16847
//
// <-- Short Description -->
// setting mark_foreground with a vector does not update individual marker colors

t = linspace(0, 2*%pi, 32);
colors1 = 1 + ceil(t/2/%pi*127);
colors2 = 1 + ceil((2*%pi-t)/2/%pi*127);

function h = drawit()
    clf
    gcf().color_map = jet(128);
    gca().axes_visible = "off";
    param3d(sin(t), cos(t), t);
    h = gce();
    h.line_mode = "off";
    h.mark_mode = "on";
    h.mark_style = 9;
    h.mark_size = 8;
endfunction

function md5 = snapshot(name)
    f = fullfile(TMPDIR, "issue_16847_" + name + ".png");
    xs2png(0, f);
    md5 = getmd5(f);
endfunction

for prop = ["mark_foreground", "mark_background"]
    // colors1 then colors2 in the same figure
    h = drawit();
    set(h, prop, colors1);
    md5_1 = snapshot(prop + "_1");
    set(h, prop, colors2);
    md5_2 = snapshot(prop + "_2");

    // colors2 directly in a new figure
    h = drawit();
    set(h, prop, colors2);
    md5_3 = snapshot(prop + "_3");

    assert_checktrue(md5_1 <> md5_2);
    assert_checkequal(md5_2, md5_3);
end

