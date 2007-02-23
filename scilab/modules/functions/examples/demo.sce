//============================
// Allan CORNET 
// INRIA 2007
//============================
// Example How to do and use a librarie
//============================
// Create a librarie with functions in SCI+'/modules/functions/examples directory
// to do once time
genlib('demofunctionslib',SCI+'/modules/functions/examples');
//============================
// load demofunctionslib
load(SCI+'/modules/functions/examples/lib');
//============================
y=3;
x=f(y);
z=g(x,y);
//============================
disp(y);
disp(x);
disp(z);
//============================
// remove all librarie and functions ref.
clear demofunctionslib y x z f g;
//============================

