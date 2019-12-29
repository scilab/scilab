// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// unitary tests of colorbar()
// =============================================================================

// ORGANIZATION of TESTS
// * Open figures 0 and 10
// * Dock them somewhere in the desk in order to make them always visible
// * Execute each block of lines and see results and comments

// CHECKING COLORS SPLITTING AND TICKING + SKIPPED ARGUMENTS
// =========================================================

// 1) Matplot
//    =======
// 1.0) Matplot: implicit minU, maxU, colminmax = [umin umax]
scf(0);
clf reset
Matplot([1 2 3;4 5 7]);
colorbar   // [1 7] graduations covered by colors #[1 7].
           // Ticks on middles of colored blocks

// 1.1) Matplot: colminmax = [umin umax] accordingly
clf
Matplot([1 2 3;4 5 7]);
colorbar(1,7,-1)  // [1 7] graduations covered by colors #[1 7].
                  // Ticks on middles of colored blocks

// 1.2)
clf
Matplot([1 2 3;4 5 7]);
colorbar(,,-1,"%6.1f")                // http://bugzilla.scilab.org/14790
assert_checkequal(gcf().children(1).ticks_format(2), "%6.1f");
// Same as above

// 1.3) Matplot: Default colminmax = [1 Nc]
clf
Matplot([1 2 3;4 5 7]);
colorbar(1,7)   // [1 7] covered with the whole colormap.
                // "1" at the very bottom. "7" at the very top.

// 1.4) Matplot: another colors range, with explicit colminmax
clf
Matplot([1 2 3;4 5 7])
colorbar(3,7, [3 7])
// Ticks 2.5-7.5 expected:
//  - integer values ticked at the middle of colors blocks
//  - other .5 values ticked at the blocks separations


// 2) Sgrayplot (Fec)
//    ==============

// Sgrayplot (Fec) : all colors used
// ---------------------------------
x = linspace(0,1,81);
z = cos(2*%pi*x)'*sin(2*%pi*x);
// 2.0) Default umin = minU, umax = maxU, colminmax = [1 Nc]
n = 4;
clf
gcf().color_map = jetcolormap(n);
Sgrayplot(x, x, z);
contour(x,x,z,[-0.5 0 0.5]);
gce().children.children(1:2:$-1).foreground=-1; // contours in black
colorbar;
// * 4 colors of equal spans
// * "-1" tick at the very bottom of the scale
// * "1" tick at the very top
// * Nice subticks
// * The contours levels must be at the right levels on the color bar

// 2.1) Default umin = minU, umax = maxU, colminmax = [1 Nc] (same as above)
n = 10;
clf
gcf().color_map = jetcolormap(n);
Sgrayplot(x, x, z);
contour(x,x,z,[-0.8 -0.6 -0.4 -0.2 0 0.2 0.4 0.6 0.8]);
gce().children.children(1:2:$-1).foreground=-1; // contours in black
colorbar;  // Same as above, with n = 10
           // Nice subticks, at blocks middles & blocks limits

// Sgrayplot (Fec) : partial colors range: 8 colors used over 20
// -------------------------------------------------------------
// 2.2) umin=minU and umax=maxU, covered by a subrange of colors
n = 8;
clf
gcf().color_map = jetcolormap(20);
Sgrayplot(x, x, z,colminmax=[3 n+2]);
contour(x,x,z,[-0.75 -0.5 -0.25 0 0.25 0.5 0.75]);
colorbar(-%inf,%inf,[3 n+2]);
// * The contours levels must be at the right levels on the color bar

// 2.3) Explicit umin and umax, with saturation for z values out of [umin, umax]:
clf
gcf().color_map = jetcolormap(20);
Sgrayplot(x, x, z, zminmax = [-0.6 0.8], colminmax = [5 11]);
contour(x,x,z,[-0.6 -0.4 -0.2 0.2 0.4 0.6]);
colorbar(-0.6, 0.8,[5 11]);
// Sgrayplot zminmax & colminmax matches the colorbar() ones
// The bar is correct, wrt contours levels


// 3) Fac3d
//    =====
function [zz, zz1] = plotSphere()
    r = 0.3;
    orig = [1.5 0 0];
    deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)";..
         "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)";..
         "z=r*sin(alp)+orig(3)*ones(tet)"]);
    [xx,yy,zz] = eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
    [xx1,yy1,zz1] = eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
    cc  = (xx+zz+2)*32;
    cc1 = (xx1-orig(1)+zz1/r+2)*32;
    clf
    plot3d1([xx xx1],[yy yy1],list([zz,zz1],[cc cc1]),theta=70,alpha=80,flag=[5,6,3])
    gcf().color_map = hotcolormap(128);
endfunction

// 3.0) Implicit min(u), max(u), on the whole color map
scf(10);
z = plotSphere();
colorbar;   // colorbar [black orange white] on u=[min(u)=-0.3, max(u)=0.3]

// 3.1) Implicit partial colors range, according to u-range
scf(0);
z = plotSphere();
colorbar(0.0, 0.15, -1);  // [cmin cmax]/Nc matches the U relative range
                          // Colors/Values should match on figures 0 and 10

// 3.2) umin & umax such that the relative ranges [umin, umax] and colminmax match
z = plotSphere();
colorbar(,,[64 128]);  // graduations on [0 , 0.3]
                       // Colors/Values should match on figures 0 and 10

// 3.3) umin & umax set according to partial colormap as fraction / whole u-range
z = plotSphere();
colorbar(,,[2.97 5]/6);   // ticks on [0, 0.2]
                          // colors in [orange, yellow] must match figure #10 ones

// 3.4) after graypolarplot()
clf reset
graypolarplot
colorbar
// Values from 30 to 137. Direct colors = full color map = dark red to white.

// 4) Plot3d
//    ======
function plotSample()
    t=[-4:0.04:4];
    clf
    gcf().color_map = jetcolormap(200);
    plot3d(t,t,sin(t)'*cos(t));
    e = gce();
    e.color_flag = 1;
    e.color_mode = -2;
endfunction

// 4.0) Bar graduated from minU=-1 to maxU=1 with the full colormap
scf(10);
plotSample();
colorbar;

// 4.1) Bar graduated from umin=1 to umax=2 with the full colormap
scf(0);
plotSample();
colorbar(1,2);

// 4.2) Bar graduated from -0.3 to 0.5 with the corresponding relative colors range
plotSample();
colorbar(-0.3,0.5, -1); // Colors/Values should match on figures 0 and 10

// 4.3) Bar graduated from minU=-1 to maxU=1, with colors #101 to #200
plotSample();
colorbar(-%inf,%inf,[101 200]); // [0 1] => [-1 1]

// 4.4) umin & umax range set to match the relative colors one
plotSample();
colorbar(,,[100 200]);  // graduations from 0 to 1
                        // Colors/Values should match on figures 0 and 10

// 4.5) Same as above, with colors given as fractions inside the whole colormap
plotSample();
colorbar(,,[0.6 0.8]);  // values from 0.2 to 0.6

// 4.6) Same as 4.4), with colors given with $
plotSample();
colorbar(,,[$/2 $]);  // values from 0 to 1


// CHECKING gce()
// ==============
twinkle(gce())  // The colorbar of the last test must twinkle
