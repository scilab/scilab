// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for issue 16610 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16610
//
// <-- Short Description -->
// lighting does not work with Fac3d surfaces

clf
surf();
xs2png(0, fullfile(TMPDIR, "issue_16610_unlit.png"));

l = light(position=[5 5 10]);
xs2png(0, fullfile(TMPDIR, "issue_16610_lit.png"));

md5_unlit = getmd5(fullfile(TMPDIR, "issue_16610_unlit.png"));
md5_lit = getmd5(fullfile(TMPDIR, "issue_16610_lit.png"));

// the light must change the rendering of the surface
assert_checktrue(md5_unlit <> md5_lit);
