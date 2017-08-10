//<-- CLI SHELL MODE -->
// <-- Non-regression test for bug 2344 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2344
//
// <-- Short Description -->
//    dassl interface bug with the info parameter
//
//    Whenever a non default info parameter is given to dassl, a wrong error 
//    pops up.
//
//    This has been reported first in the Scilab newsgroup:
//
//    http://groups.google.com/group/comp.soft-
//    sys.math.scilab/browse_frm/thread/ad556ab9e14f3005/c20f3f4f6031f338
//
//    Francois

// Author : Francois Vogel
// Date   : 16 March 2007
//
// Extracted and expanded from dassldasrt.tst

//X1DOT + 10.0*X1 = 0
//X1 + X2 = 1
//X1(0) = 1.0, X2(0) = 0.0

t    = 1:10;
t0   = 0;
y0   = [1;0];
y0d  = [-10;0];
info = list([],0,[],[],[],0,0);

deff('[r,ires]=dres1(t,y,ydot)','r=[ydot(1)+10*y(1);y(2)+y(1)-1];ires=0')

ml      = 1;
mu      = 1;
info(3) =[ml,mu];

// recorded message is cleared. if we have one ...
lasterror(%t);

execstr("yy0=dassl([y0,y0d],t0,t,dres1,info);","errcatch");
error_str = lasterror();
if error_str <> []  then pause,end
