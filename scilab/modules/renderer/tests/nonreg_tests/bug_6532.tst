// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre LANDO
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6532 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6532
//
// <-- Short Description -->
// In some case text entity rendering order was wrong.


// Run this script.


plot(1:10);
xstring(5.5,5.5,'toto');s1=gce();s1.font_style = 9
xstring(5,5,['qsdfsdf';'sqdsd';'QQQQQ'])
s=gce();s.box='on';s.background=color(255,255,238);s.fill_mode='on';

// Here you should just see a part of the last 'o' of 'toto'


a=gca();
a.rotation_angles = [-1 ,270];

// Here you should see 'toto'

a.rotation_angles = [0 ,270];

// Here you should just see a part of the last 'o' of 'toto'

