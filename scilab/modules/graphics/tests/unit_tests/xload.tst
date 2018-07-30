// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================
// unit tests for xload()

// <-- INTERACTIVE TEST -->

// Preparing a first file
path = TMPDIR+"/xload";
mkdir(path);
graypolar = path+"/graypolar.dat";
scf();
graypolarplot();   // does not use the default colormap
gca().axes_bounds = [0.5 0 0.5 1];  // puts the plot as subplot(1,2,2)
xsave(graypolar)

// 1) Loading a whole figure in a new window
// -----------------------------------------
xdel(winsid())
plot2d()            // #0
xload(graypolar, 10)
// ----- CHECK -----
// * plot2d() must be as usual, in its figure #0
// * the polarplot must be in figure #10, in subplot(1,2,2), with its true
//   colors (not the default colormap)


// 2) Loading a figure into the gcf() existing one (merging, since Scilab 6.0)
// ---------------------------------------------------------------------------
xdel(winsid())
scf();  // #0
subplot(1,2,1)
plot2d()
xload(graypolar) // in gcf()
// ----- CHECK -----
// * plot2d() is in subplot(1,2,1) with its true usual colors
// * the loaded graypolarplot must be in subplot(1,2,2).
//   Its original colors are replaced with the default ones (from plot2d())
//   = orange disk + black axes background + black frame and radial values

// 3) Same as 2) with the explicit number of an existing figure
// ------------------------------------------------------------
xdel(winsid())
subplot(1,2,1) // #0
plot2d()
scf(7);
plot()   // = gcf()
xload(graypolar, 0) // NOT in gcf()
// ----- CHECK -----
// * the figure #7 must be unchanged = usual plot() example
// * figure #0 : == previous test:
//    - plot2d() with its normal colors on the left
//    - graypolarplot() with poor (default plot2d()) colors on the right

// 4) Loading a figure into an empty gcf()
// ---------------------------------------
xdel(winsid())
scf();  // preopened empty #0
// Please dock the figure #0 anywhere you want
xload(graypolar)
assert_checkequal(length(gcf().children), 1); // the default axes must have been deleted
// ----- CHECK -----
// * only one window = figure #0, still docked at its initial place
// * the loaded graypolarplot must be in subplot(1,2,2), with its true colors
//   (not the default colormap)

// 5) When merging the loaded figure into an existing one, the children must
//    be copied in the right order
//    http://mailinglists.scilab.org/Scilab-users-Legends-of-saved-SciLab-graphs-slip-away-tp4038074p4038077.html
// ------------------------------------------------------------------------
// Preparing the file
legended = path+"/legended.dat";
xdel(winsid())
h = scf();
x=-1:0.01:1; y=sin(2*%pi*x); z=cos(2*%pi*x);
subplot(1,2,2)
plot2d(x',[y;z]',style=1:2)
legends(['Sin','Cos'],1:2,"ur")
save(legended, "h")
//  5.a) in a new scf() figure
xdel(winsid())
scf();  // Here you may dock the figure
xload(legended)
// ----- CHECK -----
// * only one window = figure #0, still docked at its initial place
// * the plot must be in subplot(1,2,2), with a visible legend block on the top right

//  5.b) in an existing non empty figure
xdel(winsid())
subplot(1,2,1)
plot2d()
xload(legended)
// ----- CHECK ----- == the .a case

// ==================
// Cleaning
rmdir(path, "s");
