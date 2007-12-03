// <-- Non-regression test for bug 612 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=612
//
// <-- Short Description -->
//    under unix the dimension of graphic window is limited to 
//    2^16, if one set dimensions higher than this the effective
//    dimension,  the value really assigned is modulo 2^16
//
//    The pb does not exist under windows

xset("wresize",0);
xset('wdim',2^16+3,5000);

if ~MSDOS then
	resultLinux   = xget("wdim");
	resultWindows = [ 65535.    5000.];
	if or(resultLinux <> resultWindows) then pause,end
end
