// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Stephane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15657 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/15657
//
// <-- Short Description -->
// strange gray ramp in histogram with histplot()

// Disable anti-aliasing to avoid minor differences in rendering linked to numerical precision
// f1.data_bounds = [1,4.847D-27;2,1.5] vs f2.data_bounds = [1,0;2,1.5]
old_aa = gdf().anti_aliasing;
gdf().anti_aliasing = "off";

f1 = scf();

histplot(3,1:2);
h = gce().children;
cf = gcf().color_map(h.foreground,:);
cb = gcf().color_map(h.background,:);

f2 = scf();

h1 = plot([1 1 4/3 4/3],[0 1.5 1.5 0],'k');
h1.fill_mode = "on";
h1.closed = "on";
h1.foreground = addcolor(cf);
h1.background = addcolor(cb);
h1.background_alpha = h.background_alpha;

h2 = plot([5/3 5/3 2 2],[0 1.5 1.5 0],'k');
h2.fill_mode = "on";
h2.foreground = addcolor(cf);
h2.background = addcolor(cb);
h2.background_alpha = h.background_alpha;
h2.closed = "on";
gca().box = "off";

// bitmap images should be bitwise equal
xs2png(f1,fullfile(TMPDIR,"bug_15657_1.png"));
xs2png(f2,fullfile(TMPDIR,"bug_15657_2.png"));
res1  =  getmd5(fullfile(TMPDIR,"bug_15657_1.png"));
res2  =  getmd5(fullfile(TMPDIR,"bug_15657_2.png"));
assert_checkequal(res1,res2);

gdf().anti_aliasing = old_aa;

