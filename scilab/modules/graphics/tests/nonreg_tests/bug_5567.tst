// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5567 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/5567
//
// <-- Short Description -->
// * bar() did not allow skipping the bar width input with []
// * bar() did not allow to specify multiple colors for multiple bars series
// * Only 10 colors names were accepted instead of the full list of predefined
//   color names.
//
clf
y = [1 -3  5];

subplot(2,4,1)
bar(y, "wheat"); // default x = [1 2 3]

subplot(2,4,2)
bar(1,[1 2 3]); // default colors (not bugged)

subplot(2,4,3)
bar(1,[2 3 4],"pink")   // single named color => replicated

subplot(2,4,4)
bar(1,y, ["green" "orange" "#AA6655"]);
//bar(1,[1 2 3],"stacked"); // default colors
// starts plotting at ymin=1 instead of 0 == http://bugzilla.scilab.org/15403


x = [1 2 5];
y = [1 -5 6
     3 -2 7
     4 -3 8 ];
subplot(2,4,5)
bar(x,y, [], ["green" "#55AA31" "orange"]);

subplot(2,4,6)
bar(x,y, [], ["green" "orange" "yellow" "wheat"]); // extra colors ignored (wheat)

subplot(2,4,7)
x = [1 2 5 6];
y = [1 4 7
     2 5 8
     2 1 0
     3 6 9];
bar(x,y, "stacked");

subplot(2,4,8)
bar(x, y, [], ["green" "#55AA31" "orange"], "stacked");

 
