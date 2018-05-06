// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test of locate()

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

scf(0);
clf
x = 1:0.1:10;

// Logarithmic and reverse cases (high demand)
plot2d("ll", x, 2.^x)
xgrid(color("grey70")*[1 1], [1 1], [7 7]);
gca().axes_reverse(1) = "on";
gca().tight_limits = "on";

locate(-1,1)
// 1) Click right on the figure
//   => CHECK:
//      a) locate must return []
//      b) the figure must be unchanged (no cross displayed)
locate(-1,1)
// 2) Click left on 3 nodes of the grid (simple coordinates).
//   => CHECK:
//      c) a X cross must be displayed on each clicked point
// 3) Then click right on the figure.
//    => CHECK:
//      d) locate must return a vector of 2 rows and 3 columns
//         coordinates must match clicked points
//      e) the 3 crosses must be removed, no longer displayed
scf(5);
clf
subplot(2,1,2), plot2d(), xgrid(color("grey70")*[1 1], [1 1], [7 7]);
subplot(2,1,1), plot2d(), xgrid(color("grey70")*[1 1], [1 1], [7 7]);
locate(-1,1)
// 4) Click left on a point in the axes of the FIRST figure #0
//    => CHECK:
//      f) no cross must appear anywhere, neither where you have
//         clicked, nor at the corresponding position on the
//         figure #5
// 5) Click left on a point on the figure #5, in the BOTTOM AXES
//    => CHECK:
//      g) no cross must appear where you have clicked, since this
//         axes is not the current one
// 6) Click left on 2 points (with simple coordinates) on the
//    figure #5, in the TOP AXES
//    => CHECK:
//      h) a X cross must appear where you have clicked
// 7) Then click right on the figure #5.
//    => CHECK:
//      i) locate must return a vector of 2 rows and 2 columns.
//         Returned coordinates must match clicked points
//      j) Both crosses must be removed, no longer displayed
locate(-1,1)
// 8) Click right on the figure #0 (the current one being #5).
//    => CHECK:
//      k) locate must return []
//         This behavior is debattable: locate() could wait the right
//         figure to be right-clicked. But it is safer in practical,
//         noticeably if many figures are opened: No matter where
//         the user has clicked: he/she wants to quit.
locate(-1,1)
// 9) Click right anyhere on the figure #5
//    => CHECK:
//      L) locate must return []
locate(-1,1)
// 10) Click left on 3 points on the figure #5, in the TOP AXES
//     Then click right on the figure #5
//    => CHECK:
//      m) locate must return a vector of 2 rows and 3 columns.

// ----------------------------------
//  LIMITED NUMBER OF EXPECTED POINTS
// ----------------------------------
clf
plot2d()
locate(3,1)
// 11) Click left on 2 points in the current figure #5
//     Then click right on any figure
//    => CHECK:
//      n) locate() must NOT QUIT, still expecting for the third point
// 12) Click left on a 3rd point in the current figure #5
//    => CHECK:
//      o) locate() must QUIT without right clicking.
//         A vector of 2 rows and 3 columns must be returned.

// -----------------
//  CLOSING FIGURES
// -----------------
locate(3,1)
// 13) Click left on 1 point in the current figure #5.
//     Close the figure #0 with its top right cross
//    => CHECK:
//     p) locate() must still wait (figure #0 was not the current one)
// 14) Close the figure #5 with its top right cross
//    => CHECK:
//     q) locate() must return []

scf(0), clf
plot2d()
locate(-1,1)
// 15) Click left on 1 point in the current figure #0.
//     Close the figure #0 with its top right cross
//    => CHECK:
//     r) locate() must return one [x;y] column of coordinates

// -------------------------------------------------------------------
xdel([0 10])
