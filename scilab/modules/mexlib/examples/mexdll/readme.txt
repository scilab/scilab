Linking a .mexglx, say xtimesy.mexglx, file with Scilab.
VISUAL C++ environment.

exec(SCI+'/modules/mexlib/examples/mexdll/demo.sce');

Note that for each mex .dll file a second .dll file must be
constructed. It is not possible to call several mex .dll
files by a unique gateway (such as libtst).
