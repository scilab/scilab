// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// Unit test for replot()

former_driver = driver("null");
xinit(TMPDIR+"/unittest.gif");
x = linspace(0,20,200);
plot2d(x,sin(x))

// Testing wrong syntaxes
// ----------------------
// Wrong argins order:
assert_checktrue(execstr("replot(gca(), [1 2 3 4])", "errcatch")~=0);
assert_checktrue(execstr("replot(""tight"", [1 2 3 4])", "errcatch")~=0);
assert_checktrue(execstr("replot(""tight"", gca())", "errcatch")~=0);
assert_checktrue(execstr("replot([1 2 3 4], ""on"", gca())", "errcatch")~=0);
assert_checktrue(execstr("replot(gca(), ""on"", [1 2 3 4])", "errcatch")~=0);

// Wrong argins values:
assert_checktrue(execstr("replot([1 2 3])", "errcatch")~=0);
assert_checktrue(execstr("replot(""about"")", "errcatch")~=0);

// Some valid syntaxes
// -------------------
assert_checktrue(execstr("replot()", "errcatch")==0);
assert_checktrue(execstr("replot(gca())", "errcatch")==0);
assert_checktrue(execstr("replot(gcf())", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2])", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], gca())", "errcatch")==0);
assert_checktrue(execstr("replot(""on"")", "errcatch")==0);
assert_checktrue(execstr("replot(gca(), ""on"")", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], ""on"")", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], ""tight"")", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], ""off"")", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], [""tight"" ""off""])", "errcatch")==0);
assert_checktrue(execstr("replot([-1 -1 2 2], gca(), ""on"")", "errcatch")==0);

// Testing set bounds
// ------------------
replot([2 0 14 0.7]);
assert_checkequal(gca().data_bounds, [2 0 ; 14 0.7]);
replot();
assert_checkalmostequal(gca().data_bounds, [0 -1 ; 20 1], 1e-3);
clf
plot2d(x,sin(x))
replot([%nan 0 14 0.7]);
assert_checkalmostequal(gca().data_bounds, [0 0 ; 14 0.7], 1e-3);
replot([3 -1 %inf 0.5]);
assert_checkalmostequal(gca().data_bounds, [3 -1 ; 20 0.5], 1e-3);

// ============================================================================
xend();
driver(former_driver);

