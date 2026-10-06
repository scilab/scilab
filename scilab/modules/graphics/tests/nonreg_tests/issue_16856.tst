// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for issue 16856 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16856
//
// <-- Short Description -->
// Matplot wrongly handles 0 entries: entries outside the colormap must not be displayed

clf
gcf().color_map = jet(3);
// hide axes because different stacking between Windows and others
gca().axes_visible = "off";

function md5 = matplot_md5(a, name)
    delete(gca().children);
    Matplot(a);
    f = fullfile(TMPDIR, "issue_16856_" + name + ".png");
    xs2png(0, f);
    md5 = getmd5(f);
endfunction

// 4 is above the colormap range (already not displayed)
md5_4 = matplot_md5([4 1; 2 3], "4");

// all entries in the colormap: the first rectangle must be displayed
md5_3 = matplot_md5([3 1; 2 3], "3");
assert_checkfalse(md5_3 == md5_4);

// 0 is below the colormap range
assert_checkequal(matplot_md5([0 1; 2 3], "0"), md5_4);

// negative entry
assert_checkequal(matplot_md5([-2 1; 2 3], "m2"), md5_4);

// non integer entries are truncated like int(a)
assert_checkequal(matplot_md5([0.5 1; 2 3], "0_5"), md5_4);
assert_checkequal(matplot_md5([3.5 1; 2 3], "3_5"), md5_3);
