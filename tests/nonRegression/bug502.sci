// <-- Non-regression test for bug 502 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=502
//
// <-- Short Description -->
//    Bug Report Id: 12442200382102810
//     I compiled Scilab-2.7.
//
//    On Scilab 2.7 with " GraphicWindow " function
//    The Error Messages are:
//      If the Graphic Window is "interfered" with,
//    for example moved, crosses path of another object
//    or even when graphic Window drop down box retracts,
//    then either the x-scale or y-scale of the 2d plot changes.
//    Commands:  An example like this did not show problem:
//    x=1:0.1:25;
//    y=x.^2;
//    plot2d(x,y);
//     Problem occurred, for example, with this:
//    x=-25:0.1:25;
//    y=x.^2
//    plot2d(x,y)
//
//
//
// ...


// set old_style on;
// remove old style graphics mode on trunk

x=-25:0.1:25;
y=x.^2;
plot2d(x,y);
result=execstr("for k=1:20,xclear();xtape(""replay"",0),xpause(1d5),end","errcatch","n");
xdel();

if result==0 then
	affich_result(%T,502);
else
	affich_result(%F,502);
end

// The Error Messages are:
// If the Graphic Window is "interfered" with,
// for example moved, crosses path of another object
// or even when graphic Window drop down box retracts,
// then either the x-scale or y-scale of the 2d plot changes.
