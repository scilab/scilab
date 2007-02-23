// Allan CORNET
// INRIA 2007

exec(SCI+'/modules/mexlib/examples/cppmex/builder.sce');


exec(SCI+'/modules/mexlib/examples/cppmex/loader.sce');


x=56;
y = square(x);
if norm(y-x^2) > %eps then pause,end
